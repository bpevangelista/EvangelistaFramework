/**
 * Copyright (C) 2012 Bruno P. Evangelista. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 * 
 * THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include "Foundation/efwPlatform.h"

namespace efw
{
namespace Graphics
{
	namespace ImageDDS
	{
#if defined(PLATFORM_BIGENDIAN)
		#define MAKE_FOURCC(a,b,c,d) ((int32_t)d | ((int32_t)c << 8) | ((int32_t)b << 16) | ((int32_t)a << 24))		
#else
		#define MAKE_FOURCC(a,b,c,d) ((int32_t)a | ((int32_t)b << 8) | ((int32_t)c << 16) | ((int32_t)d << 24))
#endif

		const int32_t kFileSignature = MAKE_FOURCC('D','D','S',' ');
		const int32_t kHeaderSize = 124;
		const int32_t kHeaderDX10Size = 32;

		// Pixel format flags
		const int32_t kPixelFormatFlags_HasAlpha = 0x1;
		const int32_t kPixelFormatFlags_IsAlphaOnlyDeprecated = 0x2;
		const int32_t kPixelFormatFlags_IsFourCC = 0x4;
		const int32_t kPixelFormatFlags_IsRGB = 0x40;
		const int32_t kPixelFormatFlags_IsYUVDeprecated = 0x200;
		const int32_t kPixelFormatFlags_IsLuminance = 0x20000;

		// Pixel format fourCC types
		const int32_t kPixelFormatFourCC_DXT1 = MAKE_FOURCC('D','X','T','1');
		const int32_t kPixelFormatFourCC_DXT2 = MAKE_FOURCC('D','X','T','2');
		const int32_t kPixelFormatFourCC_DXT3 = MAKE_FOURCC('D','X','T','3');
		const int32_t kPixelFormatFourCC_DXT4 = MAKE_FOURCC('D','X','T','4');
		const int32_t kPixelFormatFourCC_DXT5 = MAKE_FOURCC('D','X','T','5');
		const int32_t kPixelFormatFourCC_DX10 = MAKE_FOURCC('D','X','1','0');


		EFW_PACKED_BEGIN struct PixelFormatHeader
		{
			int32_t size;
			int32_t flags;
			int32_t fourCC;
			int32_t rgbaBitCount;
			int32_t redBitMask;
			int32_t greenBitMask;
			int32_t blueBitMask;
			int32_t alphaBitMask;
		} EFW_PACKED_END;


		EFW_PACKED_BEGIN struct Header
		{
			int32_t signature;
			int32_t size;
			int32_t flags;
			int32_t height;
			int32_t width;
			int32_t pitchOrLinearSize;
			int32_t depth;
			int32_t mipMapCount;
			int32_t _reserved1[11];
			PixelFormatHeader pixelFormat;
			int32_t caps1;
			int32_t caps2;
			int32_t caps3;
			int32_t caps4;
			int32_t _reserved2;
		} EFW_PACKED_END;


		EFW_PACKED_BEGIN struct DX10Header
		{
			int32_t dxgiFormat;
			int32_t resourceDimension;
			int32_t miscFlag;
			int32_t arraySize;
			int32_t reserved;
		} EFW_PACKED_END;

		uint16_t GetTextureFormat(Header* ddsHeader);
	} // DDS


	namespace ImageTGA
	{
		EFW_PACKED_BEGIN struct Header
		{
			uint8_t identsize;          // size of ID field that follows 18 byte header (0 usually)
			uint8_t colourmaptype;      // type of colour map 0=none, 1=has palette
			uint8_t imagetype;          // type of image 0=none,1=indexed,2=rgb/rgba,3=grey,+8=rle packed

			uint16_t colourmapstart;     // first colour map entry in palette
			uint16_t colourmaplength;    // number of colours in palette
			uint8_t colourmapbits;       // number of bits per palette entry 15,16,24,32

			uint16_t xstart;             // image x origin
			uint16_t ystart;             // image y origin
			uint16_t width;              // image width in pixels
			uint16_t height;             // image height in pixels
			uint8_t bits;                // image bits per pixel 8,16,24,32
			uint8_t descriptor;          // image descriptor bits (vh flip bits)
		} EFW_PACKED_END;

		uint16_t GetTextureFormat(Header* tgaHeader);

	} // TGA


} // Graphics
} // efw