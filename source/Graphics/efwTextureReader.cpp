#include "Graphics/efwTextureReader.h"
#include "Graphics/efwImageTypes.h"
#include "Foundation/efwMemory.h"
#include "Math/efwMath.h"

using namespace efw;
using namespace efw::Graphics;

void TextureReader::Release(Texture* outTexture)
{
	if (outTexture == NULL)
		return;

	SAFE_ALIGNED_FREE(outTexture->data);
}


int32_t TextureReader::CalculatePitch(int32_t width, int32_t textureFormat)
{
	switch (textureFormat)
	{
		case TextureFormats::kRGB:
			return width * 3;

		case TextureFormats::kABGR:
		case TextureFormats::kRGBA:
			return width * 4;

		case TextureFormats::kDXT1:
			return Math::Max(1, (width+3) / 4) * 8;

		case TextureFormats::kDXT3:
		case TextureFormats::kDXT5:
			return Math::Max(1, (width+3) / 4) * 16;
			break;

		default:
			EFW_ASSERT(false);
			return 0;
	};
}


int32_t TextureReader::CalculateSize(int32_t width, int32_t height, int32_t depth, int32_t mipCount, int32_t textureFormat)
{
	int32_t layerSize = 0;

	do
	{
		int32_t pitch = CalculatePitch(width, textureFormat);

		int32_t heightOrBlockCount = height;
		if (textureFormat == TextureFormats::kDXT1 ||
			textureFormat == TextureFormats::kDXT3 ||
			textureFormat == TextureFormats::kDXT5)
		{
			heightOrBlockCount = Math::Max(1, (height+3) / 4);
		}

		layerSize += pitch * heightOrBlockCount;
		mipCount--;
		width >>= 1;
		height >>= 1;
	} while (mipCount > 0);
	
	return layerSize * depth;
}


int32_t TextureReader::GetTextureFileType(int32_t* outTextureFileType, const char* textureName)
{
	FileInfo fileInfo;
	File::GetInfo(&fileInfo, textureName);
	if (!fileInfo.exists)
	{
		*outTextureFileType = TextureFileTypes::kUnknown;
		return efwErrs::kInvalidInput;
	}

	char buffer[64];
	FileReader::Read(&buffer, 64, textureName);

	// TODO With the first 64B of the file we should be able to know which texture type it's
	*outTextureFileType = TextureFileTypes::kTGA;

	return efwErrs::kOk;
}


int32_t TextureReader::ReadImage(Texture** outTexture, const char* filename)
{
	*outTexture = NULL;

	int32_t textureFileType = 0;
	GetTextureFileType(&textureFileType, filename);

	switch (textureFileType)
	{
	case TextureFileTypes::kBMP:
		// TODO Not implemented yet
		EFW_ASSERT(false);
		break;

	case TextureFileTypes::kTGA:
		return ReadTGA(outTexture, filename);
		break;

	case TextureFileTypes::kDDS:
		return ReadDDS(outTexture, filename);
		break;

	default:
		return efwErrs::kInvalidInput;
		break;
	};

	return efwErrs::kOk;
}


int32_t TextureReader::ReadTGA(Texture** outTexture, const char* filename, int32_t requiredDataAlignment)
{
	void* textureFileData = NULL;
	int32_t textureFileSize = 0;

	FileReader::ReadAll(&textureFileData, &textureFileSize, filename);
	if (textureFileData == NULL)
		return efwErrs::kInvalidInput;
		
	ImageTGA::Header* tgaHeader = (ImageTGA::Header*)textureFileData;
	void* imageData = (uint8_t*)textureFileData + sizeof(ImageTGA::Header);

	// Fix endianess
	tgaHeader->colourmapstart = efwEndianSwapIfRequired(tgaHeader->colourmapstart);
	tgaHeader->colourmaplength = efwEndianSwapIfRequired(tgaHeader->colourmaplength);
	tgaHeader->xstart = efwEndianSwapIfRequired(tgaHeader->xstart);
	tgaHeader->ystart = efwEndianSwapIfRequired(tgaHeader->ystart);
	tgaHeader->width = efwEndianSwapIfRequired(tgaHeader->width);
	tgaHeader->height = efwEndianSwapIfRequired(tgaHeader->height);

	int32_t textureBpp = (tgaHeader->bits >> 3);
	int32_t imagePitch = tgaHeader->width * textureBpp;
	int32_t imageDataSize = imagePitch * tgaHeader->height;

	// Validate TGA image
	const uint8_t kImageTypeRGB = 2;
	bool isValidTexture = 
		(tgaHeader->imagetype == kImageTypeRGB) &&
		(tgaHeader->bits == 32);
	EFW_ASSERT(isValidTexture);

	// Copy image data to VRAM
	void* textureData = memalign(requiredDataAlignment, imageDataSize);
	memcpy(textureData, imageData, imageDataSize);

	// Convert BGRA to RGBA
	EFW_ASSERT(imageDataSize%4==0);
	for (int i=0; i<imageDataSize; i+=4)
	{
		uint8_t* textureDataU8 = &((uint8_t*)textureData)[i];
		uint8_t temp = textureDataU8[0];
		textureDataU8[0] = textureDataU8[2];
		textureDataU8[2] = temp;
	}

	// Copy out
	Texture* result = (Texture*)memalign(16, sizeof(Texture));
	result->desc.width = tgaHeader->width;
	result->desc.height = tgaHeader->height;
	result->desc.depth = 1;
	result->desc.pitch = imagePitch;
	result->desc.mipCount = 1;
	result->desc.format = TextureFormats::kRGBA;
	result->dataSize = imageDataSize;
	result->data = textureData;
	*outTexture = result;

	SAFE_ALIGNED_FREE(textureFileData);
	return efwErrs::kOk;
}


int32_t TextureReader::ReadDDS(Texture** outTexture, const char* filename, int32_t requiredDataAlignment)
{
	ImageDDS::Header* ddsHeader = NULL;
	ImageDDS::DX10Header* ddsDX10Header = NULL;
	void* imageData = NULL;
	void* textureFileData = NULL;
	int32_t textureFileSize = 0;

	FileReader::ReadAll(&textureFileData, &textureFileSize, filename);
	if (textureFileData != NULL && textureFileSize > sizeof(ImageDDS::Header))
		ddsHeader = (ImageDDS::Header*)textureFileData;
	
	if (textureFileData == NULL || ddsHeader->signature != ImageDDS::kFileSignature || ddsHeader->size != efwEndianSwapIfRequired(ImageDDS::kHeaderSize))
	{
		SAFE_ALIGNED_FREE(textureFileData);
		return efwErrs::kInvalidInput;
	}

	// Endian swap
	EFW_ASSERT(sizeof(ImageDDS::Header) % 4 == 0);
	for (int i=0; i<sizeof(ImageDDS::Header)/4; i++)
	{
		int32_t* data = &((int32_t*)ddsHeader)[i];
		*data = efwEndianSwapIfRequired(*data);
	}

	// Check flags
	if ((ddsHeader->pixelFormat.flags & ImageDDS::kPixelFormatFlags_IsFourCC) != 0 &&
		ddsHeader->pixelFormat.fourCC == ImageDDS::kPixelFormatFourCC_DX10)
	{
		ddsDX10Header = (ImageDDS::DX10Header*)((uint8_t*)textureFileData + sizeof(ImageDDS::Header));
		imageData = (uint8_t*)textureFileData + sizeof(ImageDDS::Header) + sizeof(ImageDDS::DX10Header);
	}
	else
	{
		imageData = (uint8_t*)textureFileData + sizeof(ImageDDS::Header);
	}

	// Endian swap
	if (ddsDX10Header != NULL)
	{
		EFW_ASSERT(sizeof(ImageDDS::DX10Header) % 4 == 0);
		for (int i=0; i<sizeof(ImageDDS::DX10Header)/4; i++)
		{
			int32_t* data = &((int32_t*)ddsDX10Header)[i];
			*data = efwEndianSwapIfRequired(*data);
		}
	}

	// Get image desc
	int32_t width = Math::Max(1, ddsHeader->width);
	int32_t height = Math::Max(1, ddsHeader->height);
	int32_t depth = Math::Max(1, ddsHeader->depth);
	int32_t mipCount = Math::Max(1, ddsHeader->mipMapCount);
	int32_t textureFormat = ImageDDS::GetTextureFormat(ddsHeader);
	int32_t imageDataPitch = CalculatePitch(width, textureFormat);
	int32_t imageDataSize = CalculateSize(width, height, depth, mipCount, textureFormat);

	int32_t checkImageDataSize = textureFileSize - ((uint32_t)imageData - (uint32_t)textureFileData);
	EFW_ASSERT(imageDataSize == checkImageDataSize);

	// Copy image data to VRAM
	void* textureData = memalign(requiredDataAlignment, imageDataSize);
	memcpy(textureData, imageData, imageDataSize);

	// Copy out
	Texture* result = (Texture*)memalign(16, sizeof(Texture));
	result->desc.width = width;
	result->desc.height = height;
	result->desc.depth = depth;
	result->desc.pitch = imageDataPitch;
	result->desc.mipCount = mipCount;
	result->desc.format = textureFormat;
	result->dataSize = imageDataSize;
	result->data = textureData;
	*outTexture = result;

	SAFE_ALIGNED_FREE(textureFileData);
	return efwErrs::kOk;
}