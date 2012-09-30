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
#include "Foundation/efwFileReader.h"
#include "Graphics/efwTexture.h"

namespace efw
{
namespace Graphics
{
	namespace TextureReader
	{
		const int32_t kDefaultTextureAlignment = 1024;
	}
	
	namespace TextureReader
	{
		void Release(Texture* outTexture);
		int32_t GetTextureFileType(int32_t* outTextureFileType, const char* textureName);
		int32_t CalculatePitch(int32_t width, int32_t textureFormat);
		int32_t CalculateSize(int32_t width, int32_t height, int32_t depth, int32_t mipCount, int32_t textureFormat);

		int32_t ReadImage(Texture** outTexture, const char* filename);
		int32_t ReadTGA(Texture** outTexture, const char* filename, int32_t requiredDataAlignment = kDefaultTextureAlignment);
		int32_t ReadDDS(Texture** outTexture, const char* filename, int32_t requiredDataAlignment = kDefaultTextureAlignment);
	}

} // Graphics
} // efw