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
#include "Foundation/efwGuid.h"

namespace efw
{
namespace Graphics
{
	namespace VertexAttributes
	{
		const uint16_t kPosition	= 0;
		const uint16_t kNormal		= 1;
		const uint16_t kTangent		= 2;
		const uint16_t kBinormal	= 3;
		const uint16_t kColor		= 4;
		const uint16_t kBlendIndex	= 5;
		const uint16_t kBlendWeight	= 6;
		const uint16_t kUv0			= 7;
		const uint16_t kUv1			= 8;
		const uint16_t kUv2			= 9;
		const uint16_t kUv3			= 10;
		const uint16_t kUv4			= 11;
		const uint16_t kUv5			= 12;
		const uint16_t kUv6			= 13;
		const uint16_t kUv7			= 14;
		const uint16_t kCount		= 15;
	}

	struct UnprocessedTriMeshVertexAttribute
	{
		uint8_t componentCount;					// Number of 32b components (0 if data is not present)
		uint8_t offset;							// Offset from start of vertex array in 32b components
		uint8_t _pad[2];
	};

	/**
	 * Intermediate format used to store unprocessed triangle meshes coming from any input format. 
	 * Meshes are stored as indexed triangle lists, vertexes as a single interleaved array and indices as a separated array.
	 * Vertex attributes are stored as 32b per-component (e.g. 128b  for XYZW), and indexes as 32b per-index.
	 */
	struct UnprocessedTriMesh
	{
		Guid materialGuid;
		Guid guid;

		UnprocessedTriMeshVertexAttribute vertexAttributes[VertexAttributes::kCount];
		uint16_t vertexStride;
		uint16_t indexStride;
		uint32_t vertexCount;
		uint32_t indexCount;
		void* vertexData;
		void* indexData;
		
		// User custom data
		uint64_t customUserDataSize;
		void* customUserData;
	};

	/**
	 * Intermediate format used to store unprocessed triangle models.
	 */
	struct UnprocessedTriModel
	{
		uint32_t meshCount;
		UnprocessedTriMesh meshes[];
	};
}
}