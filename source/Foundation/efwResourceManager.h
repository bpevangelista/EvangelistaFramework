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
	namespace ResourceTypes
	{
		enum ResourceType
		{
			kShaderProgram,
			kTexture,
			kMaterial,
			kMesh,
			kCount
		};
	}


	struct PackageHeader
	{
		uint64_t id;
		uint32_t resourceCount;

		ResourceTypeGroup resourceTypeGroup[ResourceTypes::kCount];
		ResourceDesc resourceDesc[0];
	};


	struct ResourceTypeGroup
	{
		uint64_t resourceDescStartIndex;
		uint64_t resourceDescCount;
		uint64_t resourceDataOffsetInBytes;
		uint64_t resourceDataSize;
	};


	struct ResourceDesc
	{
		uint64_t guid;
		uint32_t resourceType;
		uintptr_t dataOffset;
		uint32_t dataSize;
	};


	namespace ResourceLoader
	{
		int32_t OpenPackage(const char* filePath);
		
		int32_t ClosePackage();
		int32_t CloseAllPackages();
	}


	struct ResourceEntry
	{
		uint64_t guid;
		uintptr_t resourceOffset;
	};


	namespace ResourceManager
	{
		int32_t GetAllByType(HandleSet** outHandleSet, int32_t resourceType);
	}
}
