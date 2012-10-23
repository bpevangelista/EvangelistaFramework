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
	namespace TextureFileTypes
	{
		const int32_t kUnknown = 0;
		const int32_t kBMP = 1;
		const int32_t kDDS = 2;
		const int32_t kTGA = 3;
	}

	namespace TextureFormats
	{
		enum TextureFormat
		{
			kUnknown = 0,
			kL8 = 1,
			kRGB = 2,
			kRGBA = 3,
			kABGR = 4,
			kDXT1 = 5,
			kDXT3 = 6,
			kDXT5 = 7
		};
	}
	typedef TextureFormats::TextureFormat TextureFormat;

	// TODO Maybe separate this concept as Image2D and Texture?
	// A texture will also need to store: type (cube, volume?), filters, etc? Or should we separate samplers and texture states?
	struct TextureDesc
	{
		int32_t width;
		int32_t height;
		int32_t depth;
		int32_t mipCount;
		//int32_t arrayCount; // Future
		int32_t pitch;
		int32_t format;
	};

	struct Texture
	{
		TextureDesc desc;

		int32_t dataSize;
		void* data;
	};

} // Graphics
} // efw