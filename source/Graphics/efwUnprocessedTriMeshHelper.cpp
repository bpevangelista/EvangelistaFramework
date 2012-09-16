#include "Graphics/efwUnprocessedTriMeshHelper.h"
#include "Graphics/efwUnprocessedTriMesh.h"
#include "Math/efwMath.h"

using namespace efw::Graphics;


void UnprocessedTriMeshHelper::GenerateBoundingSphere(float* outBoudingSphere, const UnprocessedTriMesh& mesh)
{
	if (outBoudingSphere == NULL)
		return;
	if (mesh.vertexCount == 0)
	{
		outBoudingSphere[0] = outBoudingSphere[1] = outBoudingSphere[2] = outBoudingSphere[3] = 0;
		return;
	}

	// Calculate bounding sphere center
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