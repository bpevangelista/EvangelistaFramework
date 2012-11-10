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

	namespace TriMeshIndexFormats
	{
		const int32_t kInvalidFormat = 0;
		const int32_t k16b_CCW = 1;
		const int32_t kCompressed16b_CCW = 2;
	}

	//namespace TriMeshVertexFixedFormats
	//{
	//	const int32_t kInvalidFormat = 0;
	//	const int32_t kPositionX16Y16Z16 = 1;
	//	const int32_t kPositionX16Y16Z16_NormalX10Y11Z11 = 2;
	//	const int32_t kPositionX16Y16Z16_NormalX10Y11Z11_TangentX10Y11Z11 = 3;
	//}

	// How to set this data on GPU
	// How to pass this data for edge
	// Position F16F16F16X16, F32F32F32, U16U16U16X16
	// Normal F16F16 X10Y11Z11 F32F32 F32F32F32
	// Ok, so one has the gpu format, and the other don't

	// I'll have to convert this format to an edge ID before trying to use it there

	struct TriMeshVertexAttribute
	{
		uint8_t dataFormat;					// F16/F32/S16/U16/U8 Specials:X10Y11Z11
		uint8_t dataOffset;
		uint8_t componentCount;				// 1/2/3/4
		uint8_t vertexShaderRegister;		// 0..16
	};


	struct TriMeshChunk
	{
		// Can be either 16b CCW non-compressed, or 16b CCW compressed
		// Does it make sense to compress things with 6 or less indices?
		uintptr_t indexData;
		uint32_t indexCount;
		
		uint16_t primitiveType;
		//uint16_t indexFormat; // Indices are always 16b for now
	};

	
	struct TriMesh
	{
		// Pointers to data on CPU and GPU
		// Note: CPU/GPU data doesn't mean the visibility the data but its usage
		uintptr_t vertexCpuData;
		uintptr_t vertexGpuData;

		// Interleaved data on CPU
		uint16_t vertexCpuCount;
		uint16_t vertexCpuStrideInBytes;
		uint16_t vertexCpuAttributeCount;
		TriMeshVertexAttribute vertexCpuAttributes[16];

		// Interleaved data on GPU
		uint16_t vertexGpuCount;
		uint16_t vertexGpuDataStrideInBytes;
		uint16_t vertexCpuAttributeCount;
		TriMeshVertexAttribute vertexGpuAttributes[16];

		// Chunks
		uint16_t meshChunkCount;
		TriMeshChunk meshChunks[];
	};


	/*
	struct TriMesh
	{
		// Bounding sphere volume
		float boundingSphereX, boundingSphereY, boundingSphereZ, boundingSphereRadius;

		int32_t indexDataFixedFormatId;
		int32_t vertexDataAttributeCount;
		int32_t vertexDataStride;
		int32_t vertexGpuDataAttributeCount;
		int32_t vertexGpuDataStride;
		TriMeshVertexAttribute* vertexDataAttributes;
		TriMeshVertexAttribute* vertexGpuDataAttributes;

		// Chunks
		int32_t meshChunkCount;
		TriMeshChunk* meshChunks;
	};
	*/

	//struct TriModel
	//{
	//	// Bounding sphere volume
	//	float boundingSphereX, boundingSphereY, boundingSphereZ, boundingSphereRadius;

	//	int32_t meshCount;
	//	TriMesh meshes[0];
	//	uint64_t meshesMaterial[];

	//};


} // Graphics
} // efw