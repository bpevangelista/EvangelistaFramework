#include "Graphics/efwUnprocessedTriMeshHelper.h"
#include "Graphics/efwUnprocessedTriMesh.h"
#include "Math/efwMath.h"
#include "Math/efwVectorMath.h"

#include <algorithm>
#include <map>
#include <vector>

using namespace efw::Graphics;
using namespace efw::Math;

void UnprocessedTriMeshHelper::GenerateAABoundingBox(AABoundingBox* out, const UnprocessedTriMesh& mesh)
{
	if (out == NULL)
		return;

	if (mesh.vertexCount == 0)
	{
		out->min = Vec3f::kZero;
		out->max = Vec3f::kZero;
		return;
	}

	// Calculate bounding sphere center
	Vec3f min = Vec3f(FLT_MAX);
	Vec3f max = Vec3f(-FLT_MAX);

	int32_t vertexStride = mesh.vertexStride;
	float* positions = (float*)( (uint8_t*)mesh.vertexData + mesh.vertexAttributes[VertexAttributes::kPosition].offset );
	for (int32_t i=0; i<mesh.vertexCount; ++i)
	{
		Vec3f position = Vec3f(positions);
		min = Vec3Min(min, position);
		max = Vec3Max(max, position);
		positions += vertexStride/sizeof(float);
	}

	out->min = min;
	out->max = max;
}


void UnprocessedTriMeshHelper::GenerateBoundingSphere(float* outBoudingSphere, const UnprocessedTriMesh& mesh)
{
	if (outBoudingSphere == NULL)
		return;

	if (mesh.vertexCount == 0)
	{
		outBoudingSphere[0] = outBoudingSphere[1] = outBoudingSphere[2] = outBoudingSphere[3] = 0;
		return;
	}

	float minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX;
	float maxX = -FLT_MAX, maxY = -FLT_MAX, maxZ = -FLT_MAX;

	int32_t vertexStride = mesh.vertexStride;
	float* positions = (float*)( (uint8_t*)mesh.vertexData + mesh.vertexAttributes[VertexAttributes::kPosition].offset );
	//EFW_ASSERT(vertexStride % sizeof(float) == 0);

	//EFW_ASSERT(mesh.vertexAttributes[VertexAttributes::kPosition].componentCount == 3);
	for (int32_t i=0; i<mesh.vertexCount; ++i)
	{
		if (positions[0] > maxX) maxX = positions[0];
		if (positions[1] > maxY) maxY = positions[1];
		if (positions[2] > maxZ) maxZ = positions[2];
		if (positions[0] < minX) minX = positions[0];
		if (positions[1] < minY) minY = positions[1];
		if (positions[2] < minZ) minZ = positions[2];
		positions += vertexStride/sizeof(float);
	}
	outBoudingSphere[0] = (minX+maxX) * 0.5f;
	outBoudingSphere[1] = (minY+maxY) * 0.5f;
	outBoudingSphere[2] = (minZ+maxZ) * 0.5f;

	float distanceSquared = 0;
	positions = (float*)( (uint8_t*)mesh.vertexData + mesh.vertexAttributes[VertexAttributes::kPosition].offset );
	for (int32_t i=0; i<mesh.vertexCount; ++i)
	{
		float distX = positions[0] - outBoudingSphere[0];
		float distY = positions[1] - outBoudingSphere[1];
		float distZ = positions[2] - outBoudingSphere[2];
		distanceSquared = Math::Max(distanceSquared, distX*distX+distY*distY+distZ*distZ);
		positions += vertexStride/sizeof(float);
	}

	// Calculate bounding sphere radius
	outBoudingSphere[3] = Math::Sqrt(distanceSquared);

	EFW_ASSERT(outBoudingSphere[0] == outBoudingSphere[0] && 
		outBoudingSphere[1] == outBoudingSphere[1] && 
		outBoudingSphere[2] == outBoudingSphere[2] && 
		outBoudingSphere[3] == outBoudingSphere[3]);
	EFW_ASSERT(outBoudingSphere[3] != FLT_MAX);
}


void UnprocessedTriMeshHelper::MergeBoundingSphere(float* outBoudingSphere, const float* boundingSphere1, const float* boundingSphere2)
{
	if (outBoudingSphere == NULL || boundingSphere1 == NULL || boundingSphere2 == NULL)
		return;

	float radius1 = boundingSphere1[3];
	float minX1 = boundingSphere1[0] - radius1;
	float minY1 = boundingSphere1[1] - radius1;
	float minZ1 = boundingSphere1[2] - radius1;
	float maxX1 = boundingSphere1[0] + radius1;
	float maxY1 = boundingSphere1[1] + radius1;
	float maxZ1 = boundingSphere1[2] + radius1;

	float radius2 = boundingSphere2[3];
	float minX2 = boundingSphere2[0] - radius2;
	float minY2 = boundingSphere2[1] - radius2;
	float minZ2 = boundingSphere2[2] - radius2;
	float maxX2 = boundingSphere2[0] + radius2;
	float maxY2 = boundingSphere2[1] + radius2;
	float maxZ2 = boundingSphere2[2] + radius2;

	float minX = (minX1<minX2)? minX1 : minX2;
	float minY = (minY1<minY2)? minY1 : minY2;
	float minZ = (minZ1<minZ2)? minZ1 : minZ2;
	float maxX = (maxX1>maxX2)? maxX1 : maxX2;
	float maxY = (maxY1>maxY2)? maxY1 : maxY2;
	float maxZ = (maxZ1>maxZ2)? maxZ1 : maxZ2;

	outBoudingSphere[0] = (minX+maxX) * 0.5f;
	outBoudingSphere[1] = (minY+maxY) * 0.5f;
	outBoudingSphere[2] = (minZ+maxZ) * 0.5f;

	// Calculate bounding sphere radius
	float distX = maxX - outBoudingSphere[0];
	float distY = maxY - outBoudingSphere[1];
	float distZ = maxZ - outBoudingSphere[2];
	outBoudingSphere[3] = Math::Max( Math::Max(distX, distY), distZ );

	EFW_ASSERT(outBoudingSphere[0] == outBoudingSphere[0] && 
		outBoudingSphere[1] == outBoudingSphere[1] && 
		outBoudingSphere[2] == outBoudingSphere[2] &&
		outBoudingSphere[3] == outBoudingSphere[3]);
}

bool DuplicatedVerticesSort(const std::vector<int32_t>& a, const std::vector<int32_t>& b)
{
	return (a.size()>b.size());
}

void UnprocessedTriMeshHelper::MergeDuplicatedVertices(UnprocessedTriMesh* mesh, float positionDeltaThreashold, int32_t mergeDuplicateFlags)
{
	if (mesh == NULL)
		return;

	const int32_t kOctreeAxisPartions = 32;
	const int32_t kOctreePartitions = kOctreeAxisPartions*kOctreeAxisPartions*kOctreeAxisPartions;

	AABoundingBox boundingBox;
	GenerateAABoundingBox(&boundingBox, *mesh);
	Vec3f boundingBoxSize = boundingBox.max - boundingBox.min;
	boundingBoxSize = Vec3Max(boundingBoxSize * 1.01f, Vec3f(1.0f));
	Vec3f invOctreePartitionSize = Vec3f( 1.0f / (boundingBoxSize/kOctreeAxisPartions) );

	std::vector<int32_t>* octree = (std::vector<int32_t>*)memalign(16, kOctreePartitions*sizeof(std::vector<int32_t>));
	memset(octree, 0, kOctreePartitions*sizeof(std::vector<int32_t>));
	//for (int32_t i=0; i<kOctreePartitions; ++i)
	//	octree[i] = std::vector<int32_t>();

	int32_t vertexComponents = mesh->vertexStride/sizeof(float);
	float* positions = (float*)( (uint8_t*)mesh->vertexData + mesh->vertexAttributes[VertexAttributes::kPosition].offset );
	float* normals = (mesh->vertexAttributes[VertexAttributes::kNormal].componentCount == 0 || (mergeDuplicateFlags & MergeVertexFlags::kTangentPlane_Exact) == 0)? NULL :
		(float*)( (uint8_t*)mesh->vertexData + mesh->vertexAttributes[VertexAttributes::kNormal].offset );
	float* uvs = (mesh->vertexAttributes[VertexAttributes::kUv0].componentCount == 0 || (mergeDuplicateFlags & MergeVertexFlags::kUvw0_Exact) == 0)? NULL :
		(float*)( (uint8_t*)mesh->vertexData + mesh->vertexAttributes[VertexAttributes::kUv0].offset );

	// Split vertex indices into octree-buckets
	for (int32_t i=0;i<mesh->vertexCount;++i)
	{
		Vec3f position = Vec3f( &positions[i*vertexComponents] );

		Vec3f deltaVec = position - boundingBox.min;
		deltaVec *= invOctreePartitionSize;

		Vec3f neighborsVec = Vec3Floor(deltaVec + Vec3f(0.5f));
		deltaVec = Vec3Floor(deltaVec);

		float deltas[3];
		Vec3GetFloats(deltas, deltaVec);
		int32_t partitionIndex = (int32_t)deltas[0] + (int32_t)deltas[1] * kOctreeAxisPartions + (int32_t)deltas[2] * kOctreeAxisPartions*kOctreeAxisPartions;
		octree[partitionIndex].push_back(i);

		// Get neighbor index
		neighborsVec -= deltaVec;
		neighborsVec = neighborsVec*Vec3f(2.0f) -Vec3f(1.0f);
		neighborsVec += deltaVec;
		float neighbors[3];
		Vec3GetFloats(neighbors, neighborsVec);

		int32_t partitionIndexNeighborX = (int32_t)neighbors[0] + (int32_t)deltas[1] * kOctreeAxisPartions + (int32_t)deltas[2] * kOctreeAxisPartions*kOctreeAxisPartions;
		int32_t partitionIndexNeighborY = (int32_t)deltas[0] + (int32_t)neighbors[1] * kOctreeAxisPartions + (int32_t)deltas[2] * kOctreeAxisPartions*kOctreeAxisPartions;
		int32_t partitionIndexNeighborZ = (int32_t)deltas[0] + (int32_t)deltas[1] * kOctreeAxisPartions + (int32_t)neighbors[2] * kOctreeAxisPartions*kOctreeAxisPartions;
		int32_t partitionIndexNeighborXY = (int32_t)neighbors[0] + (int32_t)neighbors[1] * kOctreeAxisPartions + (int32_t)deltas[2] * kOctreeAxisPartions*kOctreeAxisPartions;
		int32_t partitionIndexNeighborXZ = (int32_t)neighbors[0] + (int32_t)deltas[1] * kOctreeAxisPartions + (int32_t)neighbors[2] * kOctreeAxisPartions*kOctreeAxisPartions;
		int32_t partitionIndexNeighborYZ = (int32_t)deltas[0] + (int32_t)neighbors[1] * kOctreeAxisPartions + (int32_t)neighbors[2] * kOctreeAxisPartions*kOctreeAxisPartions;
		int32_t partitionIndexNeighborXYZ = (int32_t)neighbors[0] + (int32_t)neighbors[1] * kOctreeAxisPartions + (int32_t)neighbors[2] * kOctreeAxisPartions*kOctreeAxisPartions;

		bool passX = (neighbors[0] >=0 && neighbors[0] < kOctreeAxisPartions);
		bool passY = (neighbors[1] >=0 && neighbors[1] < kOctreeAxisPartions);
		bool passZ = (neighbors[2] >=0 && neighbors[2] < kOctreeAxisPartions);
		if (passX) octree[partitionIndexNeighborX].push_back(i);
		if (passY) octree[partitionIndexNeighborY].push_back(i);
		if (passZ) octree[partitionIndexNeighborZ].push_back(i);
		if (passX&passY) octree[partitionIndexNeighborXY].push_back(i);
		if (passX&passZ) octree[partitionIndexNeighborXZ].push_back(i);
		if (passY&passZ) octree[partitionIndexNeighborYZ].push_back(i);
		if (passX&passY&passZ) octree[partitionIndexNeighborXYZ].push_back(i);
	}

	// For each vertex, store an array of indices of other vertices that are near and can be merged
	int32_t duplicateIndex = 0;
	std::vector<std::vector<int32_t>> duplicates;
	duplicates.resize(mesh->vertexCount);
	for (int32_t i=0; i<kOctreePartitions; ++i)
	{
		for (uint32_t j=0; j<octree[i].size(); ++j)
		{
			int32_t testIndex = octree[i][j];
			duplicates[testIndex].push_back( testIndex );
			Vec3f testPosition = Vec3f( &positions[testIndex*vertexComponents] );
			float* testNormal = (normals != NULL)? &normals[testIndex*vertexComponents] : NULL;
			float* testUv0 = (uvs != NULL)? &uvs[testIndex*vertexComponents] : NULL;

			for (uint32_t k=j+1; k<octree[i].size(); ++k)
			{
				int32_t index = octree[i][k];
				Vec3f position = Vec3f( &positions[index*vertexComponents] );
				float* normal = (normals != NULL)? &normals[index*vertexComponents] : NULL;
				float* uv0 = (uvs != NULL)? &uvs[index*vertexComponents] : NULL;
				
				// TODO Add pass tangent and binormal
				Vec3f distanceSquared = Vec3LengthSquared(position - testPosition);
				bool passPosition = (distanceSquared.X() < (positionDeltaThreashold*positionDeltaThreashold));
				bool passNormal = (normals == NULL || 
					(Math::Abs(testNormal[0]-normal[0]) <= Math::kEpsilon && Math::Abs(testNormal[1]-normal[1]) <= Math::kEpsilon && Math::Abs(testNormal[2]-normal[2]) <= Math::kEpsilon) );
				bool passUv0 = (uvs == NULL || (Math::Abs(testUv0[0] - uv0[0]) <= Math::kEpsilon && 
					Math::Abs(testUv0[1] - uv0[1]) <= Math::kEpsilon) );
				
				if (passPosition && passNormal && passUv0)
				{
					duplicates[testIndex].push_back(index);
					duplicates[index].push_back(testIndex);
				}
			}
		}
	}
	SAFE_ALIGNED_FREE(octree);

	// Sort duplicates based on count
	std::sort(duplicates.begin(), duplicates.end(), DuplicatedVerticesSort);

	bool* usedTable = (bool*)memalign(16, mesh->vertexCount * sizeof(bool));
	memset(usedTable, 0, sizeof(bool) * mesh->vertexCount);
	std::map<uint32_t, uint32_t> indexMap = std::map<uint32_t, uint32_t>();
	
	// Create new empty vertex list
	uint32_t newVertexCount = 0;
	float* newVertexData = (float*)memalign(16, mesh->vertexCount * mesh->vertexStride);
	for (int32_t i=0; i<vertexComponents; ++i)
		newVertexData[i] = 0.0f;

	for (uint32_t i=0; i<duplicates.size(); ++i)
	{
		int32_t mergeCount = 0;
		
		for (uint32_t j=0; j<duplicates[i].size(); ++j)
		{
			int vertexIndex = duplicates[i][j];
			if (usedTable[vertexIndex])
				continue;

			// Add data from current vertex to the global one
			float* meshVertexData = (float*)mesh->vertexData;
			for (int32_t k=0; k<vertexComponents; ++k)
			{
				// TODO Improve this to handle average unique 
				newVertexData[newVertexCount*vertexComponents+k] += meshVertexData[vertexIndex*vertexComponents+k];
			}

			// Create remap for index
			indexMap.insert( std::pair<uint32_t, uint32_t>(vertexIndex, newVertexCount) );
			
			usedTable[vertexIndex] = true;
			mergeCount++;
		}

		if (mergeCount > 0)
		{
			for (int32_t k=0; k<vertexComponents; ++k)
				newVertexData[newVertexCount*vertexComponents+k] /= mergeCount;
			newVertexCount++;
		}
	}
	duplicates.clear();
	SAFE_ALIGNED_FREE(usedTable);

	static uint32_t totalCount = 0;
	static uint32_t totalDCount = 0;
	totalCount += mesh->vertexCount;
	totalDCount += mesh->vertexCount - newVertexCount;

	int32_t duplicatedVertexCount = mesh->vertexCount - newVertexCount;
	printf("VCount: %d  - Duplications: %d\r\n", mesh->vertexCount, duplicatedVertexCount);
	//printf("VCount: %d  - Duplications: %d\r\n", totalCount, totalDCount);

	// Replace old vertex data with new one
	if (newVertexCount != mesh->vertexCount)
	{
		SAFE_ALIGNED_FREE(mesh->vertexData);
	
		float* newVertexDataCompact = (float*)memalign(16, newVertexCount * mesh->vertexStride);
		memcpy(newVertexDataCompact, newVertexData, newVertexCount * mesh->vertexStride);
		mesh->vertexData = newVertexDataCompact;
		mesh->vertexCount = newVertexCount;

		// Adjust index list
		uint32_t* indices = (uint32_t*)mesh->indexData;
		for (int32_t i=0; i<mesh->indexCount; ++i)
		{
			uint32_t currentIndex = indices[i];
			indices[i] = indexMap[currentIndex];
		}
	}

	SAFE_ALIGNED_FREE(newVertexData);
}