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
#include "Graphics/efwUnprocessedTriMesh.h"
#include "Graphics/efwUnprocessedMaterial.h"

namespace efw
{
namespace Graphics
{
	namespace WavefrontObjReader
	{
		// Read file function declaration
		typedef int32_t (*ReadFileFunc_t)(void** outData, int32_t* outSize, const char* filename, int32_t requiredAlignment);

		void Release(UnprocessedTriModel* model);
		void Release(UnprocessedMaterialLib* material);
		int32_t ReadModelAndMaterials(UnprocessedTriModel** outModel, UnprocessedMaterialLib** outMaterialLib, const char* fullFilePath, ReadFileFunc_t customReadFileFunction);
		int32_t ReadMaterialLib(UnprocessedMaterialLib** outMaterial, const char* fullFilePath, ReadFileFunc_t readFileFunc);

		// Deprecated
		//int32_t ReadModelFromStream(UnprocessedTriModel** outModel, const void* objFileData, uint32_t objFileDataSize);
	}

} // Graphics
} // efw