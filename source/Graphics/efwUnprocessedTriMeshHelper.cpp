#include "Foundation/efwPointerTypes.h"
#include "Graphics/efwUnprocessedTriMeshHelper.h"
#include "Graphics/efwUnprocessedTriMesh.h"
#include "Math/efwMath.h"
#include "Math/efwVectorMath.h"

#include <algorithm>
#include <map>
#include <vector>

using namespace efw;
using namespace efw::Graphics;
using namespace efw::Math;

int32_t GenerateAABoundingBox(AABoundingBox* out, const float* positions, int32_t floatComponentsPerVertex, int32_t vertexCount)
{
	if (out == NULL)
		return efwErrs::kInvalidInput;

	if (vertexCount == 0)
	{
		out->min = Vec3f::kZero;
		out->max = Vec3f::kZero;
		return efwErrs::kOk;
	}

	// Calculate bounding sphere center
	Vec3f min = Vec3f(FLT_MAX);
	Vec3f max = Vec3f(-FLT_MAX);

	for (int32_t i=0; i<vertexCount; ++i)
	{
		Vec3f position = Vec3f(positions);
		min = Vec3Min(min, position);
		max = Vec3Max(max, position);
		positions += floatComponentsPerVertex;
	}

	out->min = min;
	out->max = max;

	return efwErrs::kOk;
}

int32_t UnprocessedTriMeshHelper::GenerateAABoundingBox(AABoundingBox* out, const UnprocessedTriMesh& mesh)
{
	float* positions = (float*)( (uint8_t*)mesh.vertexData + mesh.vertexAttributes[VertexAttributes::kPosition].offset );
	int32_t floatComponentsPerVertex = mesh.vertexStride/sizeof(float);

	return ::GenerateAABoundingBox(out, positions, floatComponentsPerVertex, mesh.vertexCount);
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

int32_t UnprocessedTriMeshHelper::MergeDuplicatedVertices(UnprocessedTriMesh* mesh, float positionDeltaThreashold, int32_t mergeDuplicateFlags)
{
	if (mesh == NULL)
		return efwErrs::kInvalidInput;

	const int32_t kOctreeAxisPartions = 32;
	const int32_t kOctreePartitions = kOctreeAxisPartions*kOctreeAxisPartions*kOctreeAxisPartions;

	AABoundingBox boundingBox;
	GenerateAABoundingBox(&boundingBox, *mesh);
	Vec3f boundingBoxSize = boundingBox.max - boundingBox.min;
	boundingBoxSize = Vec3Max(boundingBoxSize * Vec3f(1.01f), Vec3f(1.0f));
	//Vec3f invOctreePartitionSize = Vec3f( Vec3f(1.0f) / (boundingBoxSize/Vec3f(kOctreeAxisPartions)) );
	Vec3f invOctreePartitionSize = Vec3f((float)kOctreeAxisPartions)/boundingBoxSize;

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
	EFW_SAFE_ALIGNED_FREE(octree);

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
	EFW_SAFE_ALIGNED_FREE(usedTable);

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
		EFW_SAFE_ALIGNED_FREE(mesh->vertexData);
	
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

	EFW_SAFE_ALIGNED_FREE(newVertexData);

	return efwErrs::kOk;
}


int32_t InternalCompressVertexAttribute(void* output, const float* input, int32_t outputComponentsPerVertex, int32_t inputComponentsPerVertex, int32_t vertexCount,
	int32_t attributeCompression, float scale[4], float bias[4])
{
	EFW_ASSERT(output != NULL && input != NULL);
	EFW_ASSERT(outputComponentsPerVertex <= inputComponentsPerVertex);
	EFW_ASSERT(outputComponentsPerVertex <= 4);

	// If there's scale and bias the value will become unsigned
	bool isSigned = (
		//attributeCompression == AttributeCompressions::kSFloatToU8NormWithScaleAndBias
		//|| attributeCompression == AttributeCompressions::kSFloatToU16NormWithScaleAndBias
		attributeCompression == AttributeCompressions::kSFloatNormToU8Norm
		|| attributeCompression == AttributeCompressions::kSFloatNormToU16Norm);

	float conversionBias = 0.0f;
	float conversionScale = 1.0f;
	if (isSigned)
	{
		conversionBias = 0.5f;
		conversionScale = 0.5f;
	}

	if (attributeCompression == AttributeCompressions::kSFloatNormToU8Norm || attributeCompression == AttributeCompressions::kUFloatNormToU8Norm ||
		attributeCompression == AttributeCompressions::kSFloatToU8NormWithScaleAndBias || attributeCompression == AttributeCompressions::kUFloatToU8NormWithScaleAndBias)
	{
		for (int32_t i=0; i<vertexCount; i++)
		{
			for (int32_t j=0; j<outputComponentsPerVertex; j++)
			{
				float value = input[i*inputComponentsPerVertex+j];
				float valueWithScaleBias = (value-bias[j])/scale[j];
				float finalValue = (valueWithScaleBias * conversionScale + conversionBias);
				EFW_ASSERT( finalValue >= 0.0f && finalValue <= 1.0f );

				((uint8_t*)output)[i*outputComponentsPerVertex+j] = (uint8_t)(finalValue * UINT8_MAX);
			}
		}
	}
	else if (attributeCompression == AttributeCompressions::kSFloatNormToU16Norm || attributeCompression == AttributeCompressions::kUFloatNormToU16Norm ||
		attributeCompression == AttributeCompressions::kSFloatToU16NormWithScaleAndBias || attributeCompression == AttributeCompressions::kUFloatToU16NormWithScaleAndBias )
	{
		for (int32_t i=0; i<vertexCount; i++)
		{
			for (int32_t j=0; j<outputComponentsPerVertex; j++)
			{
				float value = input[i*inputComponentsPerVertex+j];
				float valueWithScaleBias = (value-bias[j])/scale[j];
				float finalValue = (valueWithScaleBias * conversionScale + conversionBias);
				EFW_ASSERT( finalValue >= 0.0f && finalValue <= 1.0f );

				((uint16_t*)output)[i*outputComponentsPerVertex+j] = (uint16_t)(finalValue * UINT16_MAX);
			}
		}
	}
	else
	{
		EFW_ASSERT(false);
	}

	return efwErrs::kOk;
}


int32_t UnprocessedTriMeshHelper::CompressVertexAttribute(void** outData, const float* inputVertexData, int32_t vertexStride, int32_t vertexCount, UnprocessedTriMeshVertexAttribute attribute, 
	AttributeCompression attributeCompression, float* outPerComponentScale, float* outPerComponentBias)
{
	if (outData == NULL || inputVertexData == NULL)
		return efwErrs::kInvalidInput;

	// The maximum is 4 components
	if (attribute.componentCount > 4)
		return efwErrs::kInvalidInput;

	float scale[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	float bias[4] = {0.0f, 0.0f, 0.0f, 0.0f};

	int32_t vertexDataComponents = vertexStride/sizeof(float);
	float* vertexData = (float*)( (uint8_t*)inputVertexData + attribute.offset );
	void* newAttributeData = memalign(16, sizeof(uint16_t)*attribute.componentCount*vertexCount);

	bool useScaleAndBias = (attributeCompression == AttributeCompressions::kSFloatToU8NormWithScaleAndBias
			|| attributeCompression == AttributeCompressions::kSFloatToU16NormWithScaleAndBias
			|| attributeCompression == AttributeCompressions::kUFloatToU8NormWithScaleAndBias
			|| attributeCompression == AttributeCompressions::kUFloatToU16NormWithScaleAndBias);
	EFW_ASSERT(!useScaleAndBias || (outPerComponentBias != NULL && outPerComponentScale != NULL));

	if (useScaleAndBias)
	{
		float length[4];
		float minValue[4], maxValue[4];
		length[0] = length[1] = length[2] = length[3] = 1.0f;
		minValue[0] = minValue[1] = minValue[2] = minValue[3] = FLT_MAX;
		maxValue[0] = maxValue[1] = maxValue[2] = maxValue[3] = -FLT_MAX;
		
		// Get the min and max value for each component
		for (int32_t i=0; i<vertexCount; i++)
		{
			for (int32_t j=0; j<attribute.componentCount; j++)
			{
				float value = vertexData[i*vertexDataComponents+j];
				minValue[j] = (minValue[j]>value)? value : minValue[j];
				maxValue[j] = (maxValue[j]>value)? maxValue[j] : value;
			}
		}

		// Get the length for each component
		for (int32_t i=0; i<attribute.componentCount; i++)
		{
			length[i] = maxValue[i]-minValue[i];
			if (length[i] == 0)
				length[i] = 1.0f;
		}

		for (int32_t i=0; i<attribute.componentCount; i++)
		{
			scale[i] = length[i];
			bias[i] = minValue[i];
		}
	}

	int32_t result = InternalCompressVertexAttribute(newAttributeData, vertexData, attribute.componentCount, vertexDataComponents, vertexCount, 
		attributeCompression, scale, bias);
	
	*outData = newAttributeData;
	if (useScaleAndBias && outPerComponentBias != NULL && outPerComponentScale != NULL)
	{
		for (int32_t j=0; j<attribute.componentCount; j++)
		{
			outPerComponentBias[j] = bias[j];
			outPerComponentScale[j] = scale[j];
		}
	}

// TODO Put in a separated method? Compute error
#if 0
	// If there's scale and bias the value will become unsigned
	bool isSigned = (
		attributeCompression == AttributeCompressions::kSFloatNormToU8Norm
		|| attributeCompression == AttributeCompressions::kSFloatNormToU16Norm);

	float conversionBias = 0.0f;
	float conversionScale = 1.0f;
	if (isSigned)
	{
		conversionBias = 0.5f;
		conversionScale = 0.5f;
	}

	// Debug
	double totalError = 0.0;
	double totalErrorOverLength = 0.0;

	for (int32_t i=0; i<vertexCount; i++)
	{
		EFW_ALIGNED_TYPE(16, float) original[4] = {0.0f, 0.0f, 0.0f, 0.0f};
		EFW_ALIGNED_TYPE(16, float) compressed[4] = {0.0f, 0.0f, 0.0f, 0.0f};
			
		for (int32_t j=0; j<attribute.componentCount; j++)
		{
			original[j] = vertexData[i*vertexDataComponents+j];

			if (attributeCompression == AttributeCompressions::kSFloatNormToU8Norm || attributeCompression == AttributeCompressions::kUFloatNormToU8Norm ||
				attributeCompression == AttributeCompressions::kSFloatToU8NormWithScaleAndBias || attributeCompression == AttributeCompressions::kUFloatToU8NormWithScaleAndBias)
			{
				uint8_t* data = (uint8_t*)newAttributeData;
				compressed[j] = (data[i*attribute.componentCount+j]/(float)UINT8_MAX) * scale[j] + bias[j];
				compressed[j] = (compressed[j] - conversionBias) / conversionScale ;
			}
			else if (attributeCompression == AttributeCompressions::kSFloatNormToU16Norm || attributeCompression == AttributeCompressions::kUFloatNormToU16Norm ||
				attributeCompression == AttributeCompressions::kSFloatToU16NormWithScaleAndBias || attributeCompression == AttributeCompressions::kUFloatToU16NormWithScaleAndBias )
			{
				uint16_t* data = (uint16_t*)newAttributeData;
				compressed[j] = (data[i*attribute.componentCount+j]/(float)UINT16_MAX) * scale[j] + bias[j];
				compressed[j] = (compressed[j] - conversionBias) / conversionScale ;
			}
		}

		// TODO: Change to Vec4f
		Vec3f originalVec = Vec3f(original);
		Vec3f compressedVec = Vec3f(compressed);
		Vec3f difference = Vec3Length(compressedVec - originalVec);

		totalError += Vec3Length(difference).X();
		totalErrorOverLength += Vec3Length(difference / Vec3f(scale)).X();

		//printf("Original [%f, %f, %f]\r\n", vertexData[i*vertexDataComponents+0], vertexData[i*vertexDataComponents+1], vertexData[i*vertexDataComponents+2]);
		//printf("Compress [%f, %f, %f]\r\n", 
		//	outPerComponentBias[0] + (newAttributeData[i*attribute.componentCount+0]/(float)UINT16_MAX) *outPerComponentScale[0],
		//	outPerComponentBias[1] + (newAttributeData[i*attribute.componentCount+1]/(float)UINT16_MAX) *outPerComponentScale[1],
		//	outPerComponentBias[2] + (newAttributeData[i*attribute.componentCount+2]/(float)UINT16_MAX) *outPerComponentScale[2]);
	}
	printf("Quantization Error/ErrorOverLength: [%f, %f]\r\n", totalError, totalErrorOverLength);
#endif

	return result;
}


int32_t UnprocessedTriMeshHelper::CompressTangentSpace(void** outData, const float* inputVertexData, int32_t vertexStride, int32_t vertexCount, 
	UnprocessedTriMeshVertexAttribute vertexAttributes[VertexAttributes::kCount], TangentFrameCompression compressionType)
{
	if (outData == NULL || inputVertexData == NULL)
		return efwErrs::kInvalidInput;
	if (vertexAttributes[VertexAttributes::kNormal].componentCount != 3)
		return efwErrs::kInvalidInput;

	int32_t inputVertexComponents = vertexStride/sizeof(float);
	ScopedPtr<float> outputData;

	if (compressionType == TangentFrameCompressions::k64bNormalOnly_AzimuthalProjection)
	{
		float* inputData = (float*)( (uint8_t*)inputVertexData + vertexAttributes[VertexAttributes::kNormal].offset );
		int32_t outputComponentsPerVertex = 2;
		outputData.Reset( (float*)memalign(16, outputComponentsPerVertex*sizeof(float)*vertexCount) );

		for (int32_t i=0; i<vertexCount; i++)
		{
			float* normalValues = &inputData[i*inputVertexComponents];
			EFW_ASSERT( normalValues[0] >= -1.0f && normalValues[0] <= 1.0f);
			EFW_ASSERT( normalValues[1] >= -1.0f && normalValues[1] <= 1.0f);
			EFW_ASSERT( normalValues[2] >= -1.0f && normalValues[2] <= 1.0f);
			//normalValues[2] = normalValues[2] * 0.5f + 0.5f;

			float scaler = Math::Sqrt(8.0f*normalValues[2]+8.0f);
			//if (scaler == 0.0f)
			//	scaler = 0.5f;
			float newX = normalValues[0] / scaler + 0.5f;
			float newY = normalValues[1] / scaler + 0.5f;

			// TODO: This method needs to be fixed, including this clamp
			outputData[i*outputComponentsPerVertex+0] = Math::Clamp(newX, 0.00002f, 1.0f);
			outputData[i*outputComponentsPerVertex+1] = Math::Clamp(newY, 0.00002f, 1.0f);
			EFW_ASSERT( outputData[i*outputComponentsPerVertex+0] > 0.0f );
			EFW_ASSERT( outputData[i*outputComponentsPerVertex+1] > 0.0f );
		}
	}
	else if (compressionType == TangentFrameCompressions::k64bNormalOnly_SphereMapping)
	{
		float* inputData = (float*)( (uint8_t*)inputVertexData + vertexAttributes[VertexAttributes::kNormal].offset );
		int32_t outputComponentsPerVertex = 2;
		outputData.Reset( (float*)memalign(16, outputComponentsPerVertex*sizeof(float)*vertexCount) );

		for (int32_t i=0; i<vertexCount; i++)
		{
			float* normalValues = &inputData[i*inputVertexComponents];
			EFW_ASSERT( normalValues[0] >= -1.0f && normalValues[0] <= 1.0f);
			EFW_ASSERT( normalValues[1] >= -1.0f && normalValues[1] <= 1.0f);
			EFW_ASSERT( normalValues[2] >= -1.0f && normalValues[2] <= 1.0f);

			// Make sure X and Y are not zero
			if (fabs(normalValues[0]) < 0.00002f)
				normalValues[0] = (normalValues[0] >= 0)? 0.00002f : -0.00002f;
			if (fabs(normalValues[1]) < 0.00002f)
				normalValues[1] = (normalValues[1] >= 0)? 0.00002f : -0.00002f;

			Vec3f normalXYVec = Vec3f(normalValues[0], normalValues[1], 0);
			float lengthXY = Vec3Length(normalXYVec).X();
			EFW_ASSERT(lengthXY != 0);

			// Make sure newZ is not zero
			float newZ = normalValues[2] * 0.5f + 0.5f;
			if (fabs(newZ) < 0.00002f)
				newZ = (newZ >= 0)? 0.00002f : -0.00002f;
			float newX = normalValues[0] * Math::Sqrt(newZ) / lengthXY;
			float newY = normalValues[1] * Math::Sqrt(newZ) / lengthXY;

			// 
			outputData[i*outputComponentsPerVertex+0] = Math::Min(newX * 0.5f + 0.5f, 1.0f);
			outputData[i*outputComponentsPerVertex+1] = Math::Min(newY * 0.5f + 0.5f, 1.0f);
			EFW_ASSERT( outputData[i*outputComponentsPerVertex+0] > 0.0f );
			EFW_ASSERT( outputData[i*outputComponentsPerVertex+1] > 0.0f );
		}
	}
	else
	{
		// Not implemented yet
		EFW_ASSERT(false);
	}

	*outData = outputData.Release();

	return efwErrs::kOk;
}
