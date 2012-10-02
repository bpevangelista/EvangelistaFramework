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

#include "Math/efwVectorMath.h"

namespace efw
{

namespace Graphics
{
	// TODO BE Move it to somewhere else (physics?)
	struct AABoundingBox
	{
		Math::Vec3f min;
		Math::Vec3f max;
	};

	struct BoundingSphere
	{
		float center[3];
		float radius;
	};

	namespace MergeVertexFlags
	{
		const int32_t kTangentPlane_Exact				= 1<<1;
		const int32_t kTangentPlane_Average				= 1<<2;
		const int32_t kTangentPlane_AverageUniques		= 1<<3;

		const int32_t kUvw_Exact						= 1<<4;
		const int32_t kUvw_Average						= 1<<5;
		const int32_t kUvw_AveragaUniques				= 1<<6;
	}


	namespace UnprocessedTriMeshHelper
	{
		void GenerateAABoundingBox(AABoundingBox* outBoudingBox, const UnprocessedTriMesh& mesh);

		void GenerateBoundingSphere(float* outBoudingSphere, const UnprocessedTriMesh& mesh);
		void MergeBoundingSphere(float* outBoudingSphere, const float* boudingSphere1, const float* boudingSphere2);

		void MergeDuplicatedVertices(UnprocessedTriMesh* mesh, float positionDeltaThreashold, int32_t mergeDuplicateFlags);
	}
}

}