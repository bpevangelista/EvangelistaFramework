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
#include "Graphics/efwTexture.h"

namespace efw
{
namespace Graphics
{
	struct UnprocessedMaterial
	{
		Guid guid;
		Texture* albedoTexture;
		Texture* normalMapTexture;

		char albedoTextureFilename[Path::kMaxFullPathLength];
		char normalMapTextureFilename[Path::kMaxFullPathLength];
		//UnprocessedMaterialTexture specularTexture;
	};

	struct UnprocessedMaterialLib
	{
		int32_t materialCount;
		UnprocessedMaterial materials[];
	};
}
}