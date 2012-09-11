#include "Graphics/efwTextureReader.h"

#include "Foundation/efwMemory.h"
#include "Foundation/efwFileReader.h"
#include "Graphics/efwTexture.h"
#include "Graphics/efwUnprocessedTriMesh.h"

using namespace efw;
using namespace efw::Graphics;

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


int32_t TextureReader::ReadImage(UnprocessedMaterialTexture** outTexture, const char* filename)
{
	*outTexture = NULL;

	int32_t textureFileType = 0;
	GetTextureFileType(&textureFileType, filename);

	switch (textureFileType)
	{
	case TextureFileTypes::kBMP:
		//
		EFW_ASSERT(false);
		break;

	case TextureFileTypes::kTGA:
		return ReadTGA(outTexture, filename);
		break;

	case TextureFileTypes::kDDS:
		//
		EFW_ASSERT(false);
		break;

	default:
		return efwErrs::kInvalidInput;
		break;
	};

	return efwErrs::kOk;
}


int32_t TextureReader::ReadTGA(UnprocessedMaterialTexture** outTexture, const char* filename)
{

	PACKED( struct TGA_Header
	{
		uint8_t identsize;          // size of ID field that follows 18 byte header (0 usually)
		uint8_t colourmaptype;      // type of colour map 0=none, 1=has palette
		uint8_t imagetype;          // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed

		uint16_t colourmapstart;     // first colour map entry in palette
		uint16_t colourmaplength;    // number of colours in palette
		uint8_t colourmapbits;      // number of bits per palette entry 15,16,24,32

		uint16_t xstart;             // image x origin
		uint16_t ystart;             // image y origin
		uint16_t width;              // image width in pixels
		uint16_t height;             // image height in pixels
		uint8_t bits;               // image bits per pixel 8,16,24,32
		uint8_t descriptor;         // image descriptor bits (vh flip bits)
	} );


	void* textureFileData = NULL;
	int32_t textureFileSize = 0;

	int32_t kTextureAlignment = 1024;
	FileReader::ReadAll(&textureFileData, &textureFileSize, filename, kTextureAlignment);
	if (textureFileData == NULL)
		return efwErrs::kInvalidState;
		
	TGA_Header* tgaHeader = (TGA_Header*)textureFileData;
	void* imageData = (uint8_t*)textureFileData + sizeof(TGA_Header);

	// Fix endianess
	tgaHeader->colourmapstart = efwEndianSwap(tgaHeader->colourmapstart);
	tgaHeader->colourmaplength = efwEndianSwap(tgaHeader->colourmaplength);
	tgaHeader->xstart = efwEndianSwap(tgaHeader->xstart);
	tgaHeader->ystart = efwEndianSwap(tgaHeader->ystart);
	tgaHeader->width = efwEndianSwap(tgaHeader->width);
	tgaHeader->height = efwEndianSwap(tgaHeader->height);

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
	void* textureData = memalign(kTextureAlignment, imageDataSize);
	memcpy(textureData, imageData, imageDataSize);

	// Copy out
	UnprocessedMaterialTexture* result = (UnprocessedMaterialTexture*)memalign(16, sizeof(UnprocessedMaterialTexture));
	result->width = tgaHeader->width;
	result->height = tgaHeader->height;
	result->pitch = imagePitch;
	result->format = TextureDataFormats::kABGR;
	result->data = textureData;
	memset(result->sourceFilename, 0, COUNTOF(result->sourceFilename));
	strcpy(result->sourceFilename, filename);
	*outTexture = result;

	SAFE_ALIGNED_FREE(textureFileData);
	return efwErrs::kOk;
}
