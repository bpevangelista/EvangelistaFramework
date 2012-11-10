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
		const uint16_t kUnknown = 0;
		const uint16_t kBMP = 1;
		const uint16_t kDDS = 2;
		const uint16_t kTGA = 3;
	}

	namespace TextureFormats
	{
		const uint16_t kUnknown = 0;
		const uint16_t kL8 = 1;
		const uint16_t kRGB = 2;
		const uint16_t kRGBA = 3;
		const uint16_t kABGR = 4;
		const uint16_t kDXT1 = 5;
		const uint16_t kDXT3 = 6;
		const uint16_t kDXT5 = 7;
	}

	// TODO Maybe separate this concept as Image2D and Texture?
	// A texture will also need to store: type (cube, volume?), filters, etc? Or should we separate samplers and texture states?
	struct TextureDesc
	{
		uint16_t width;
		uint16_t height;
		uint16_t depth;
		uint16_t mipCount;
		//uint32_t arrayCount; // Future
		uint16_t pitch;
		uint16_t format;
	};

	struct Texture
	{
		uint64_t dataSize;
		void* data;

		TextureDesc desc;
	};

} // Graphics
} // efw