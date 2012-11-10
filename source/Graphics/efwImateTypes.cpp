#include "Graphics/efwImageTypes.h"
#include "Graphics/efwTexture.h"

using namespace efw;
using namespace efw::Graphics;

uint16_t ImageDDS::GetTextureFormat(ImageDDS::Header* ddsHeader)
{
	bool isLuminance = (ddsHeader->pixelFormat.flags & ImageDDS::kPixelFormatFlags_IsLuminance) != 0;
	bool isRGB = (ddsHeader->pixelFormat.flags & ImageDDS::kPixelFormatFlags_IsRGB) != 0;
	bool isFourCC = (ddsHeader->pixelFormat.flags & ImageDDS::kPixelFormatFlags_IsFourCC) != 0;
	bool hasAlpha = (ddsHeader->pixelFormat.flags & ImageDDS::kPixelFormatFlags_HasAlpha) != 0;
	
	// Right now we only support RGB, RGBA or FourCC formats
	EFW_ASSERT(isLuminance || isRGB || isFourCC);
	if (!isLuminance && !isRGB && !isFourCC)
		return TextureFormats::kUnknown;

	if (isFourCC)
	{
		switch (ddsHeader->pixelFormat.fourCC)
		{
		case ImageDDS::kPixelFormatFourCC_DXT1:
			return TextureFormats::kDXT1;

		case ImageDDS::kPixelFormatFourCC_DXT3:
			return TextureFormats::kDXT3;

		case ImageDDS::kPixelFormatFourCC_DXT5:
			return TextureFormats::kDXT5;

		// TODO Handle block compression formats in DX10
		case ImageDDS::kPixelFormatFourCC_DXT2:
		case ImageDDS::kPixelFormatFourCC_DXT4:
		case ImageDDS::kPixelFormatFourCC_DX10:
		default:
			return TextureFormats::kUnknown;
		};
	}
	else if (isRGB)
	{
		return (hasAlpha)? TextureFormats::kRGBA : TextureFormats::kRGB;
	}
	else if (isLuminance)
	{
		// TODO Is it possible to have alpha here as well?
		EFW_NOT_IMPLEMENTED_YET; // As we need to check if this path works
		return TextureFormats::kL8;
	}

	// Should never reach here
	return TextureFormats::kUnknown;
}