#include "Graphics/efwUnprocessedTriMeshHelper.h"

#include "Graphics/efwUnprocessedTriMesh.h"

#include <math.h>

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
		distanceSquared = MAX(distanceSquared, distX*distX+distY*distY+distZ*distZ);
		positions += vertexStride/sizeof(float);
	}

	// Calculate bounding sphere radius
	outBoudingSphere[3] = sqrtf(distanceSquared);

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
	outBoudingSphere[3] = MAX( MAX(distX, distY), distZ );

	EFW_ASSERT(outBoudingSphere[0] == outBoudingSphere[0] && 
		outBoudingSphere[1] == outBoudingSphere[1] && 
		outBoudingSphere[2] == outBoudingSphere[2] &&
		outBoudingSphere[3] == outBoudingSphere[3]);
}


/*
void UnprocessedTriMeshHelper::GenerateBoundingSphere(float* outBoudingSphere, const UnprocessedTriMesh& mesh)
{
		// Stores r1, an offset -0xB0*4 after it
		00019784 F821FF51 stdu       r1,-0xB0(r1)                   PIPE // move stack down 11 * 16 = 176 // store double word with update (word==32b)  (176 * 4) == 704

		// Moves from special purpoase register (LR -> R0)
		00019788 7C0802A6 mfspr      r0,lr                         02 // 

		0001978C F80100C0 std        r0,0xC0(r1)                    PIPE // store r0 data at 0xc0*4 on top of r1, have r1 moved?


		00019790 3CA00000 lis        r5,0x0
		00019794 30A500E4 addic      r5,r5,0xE4                    01 (00019790) REG PIPE
		00019798 7C61292E stwx       r3,r1,r5                      03 (00019794) REG LSU
		0001979C 3C600000 lis        r3,0x0                         PIPE
		000197A0 306300EC addic      r3,r3,0xEC                    01 (0001979C) REG
		000197A4 7C81192E stwx       r4,r1,r3                      03 (000197A0) REG PIPE LSU
		if (outBoudingSphere == NULL)
			000197A8 3C600000 lis        r3,0x0
			000197AC 306300E4 addic      r3,r3,0xE4                    01 (000197A8) REG PIPE
			000197B0 7C61182E lwzx       r3,r1,r3                      03 (000197AC) REG LSU
			000197B4 7C6307B4 extsw      r3,r3                         02 (000197B0) REG
			000197B8 2C030000 cmpwi      r3,0x0                        01 (000197B4) REG
			000197BC 40820008 bne        0x000197C4                    01 (000197B8) REG
			000197C0 48000638 b          0x00019DF8                    08
			return;
	if (mesh.vertexCount == 0)
		000197C4 3C600000 lis        r3,0x0                         PIPE
		000197C8 306300EC addic      r3,r3,0xEC                    01 (000197C4) REG
		000197CC 7C61182E lwzx       r3,r1,r3                      04 (000197C8) REG LSU
		000197D0 80630048 lwz        r3,0x48(r3)                   03 (000197CC) REG LSU
		000197D4 2C030000 cmpwi      r3,0x0                        02 (000197D0) REG
		000197D8 40820078 bne        0x00019850
	{
		outBoudingSphere[0] = outBoudingSphere[1] = outBoudingSphere[2] = outBoudingSphere[3] = 0;
		000197DC 3C600007 lis        r3,0x7
			000197E0 C02329D0 lfs        f1,0x29D0(r3)                 03 (000197DC) REG LSU
			000197E4 FC200890 fmr        f1,f1                          PIPE
			000197E8 3C600000 lis        r3,0x0
			000197EC 306300E4 addic      r3,r3,0xE4                    01 (000197E8) REG PIPE
			000197F0 7C61182E lwzx       r3,r1,r3                      03 (000197EC) REG LSU
			000197F4 D023000C stfs       f1,0xC(r3)                    03 (000197F0) REG PIPE LSU
			000197F8 3C600007 lis        r3,0x7
			000197FC C02329D4 lfs        f1,0x29D4(r3)                 04 (000197F8) REG LSU
			00019800 FC200890 fmr        f1,f1
			00019804 3C600000 lis        r3,0x0
			00019808 306300E4 addic      r3,r3,0xE4                    01 (00019804) REG
			0001980C 7C61182E lwzx       r3,r1,r3                      04 (00019808) REG LSU
			00019810 D0230008 stfs       f1,0x8(r3)                    03 (0001980C) REG LSU
			00019814 3C600007 lis        r3,0x7
			00019818 C02329D8 lfs        f1,0x29D8(r3)                 03 (00019814) REG LSU
			0001981C FC200890 fmr        f1,f1                          PIPE
			00019820 3C600000 lis        r3,0x0
			00019824 306300E4 addic      r3,r3,0xE4                    01 (00019820) REG PIPE
			00019828 7C61182E lwzx       r3,r1,r3                      03 (00019824) REG LSU
			0001982C D0230004 stfs       f1,0x4(r3)                    03 (00019828) REG PIPE LSU
			00019830 3C600007 lis        r3,0x7
			00019834 C02329DC lfs        f1,0x29DC(r3)                 04 (00019830) REG LSU
			00019838 FC200890 fmr        f1,f1
			0001983C 3C600000 lis        r3,0x0
			00019840 306300E4 addic      r3,r3,0xE4                    01 (0001983C) REG
			00019844 7C61182E lwzx       r3,r1,r3                      04 (00019840) REG LSU
			00019848 D0230000 stfs       f1,0x0(r3)                    03 (00019844) REG LSU
			0001984C 480005AC b          0x00019DF8                    08
			return;
	}

	// Calculate bounding sphere center
	float minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX;
	00019850 3C600007 lis        r3,0x7
		00019854 C02329E0 lfs        f1,0x29E0(r3)                 04 (00019850) REG LSU
		00019858 FC200890 fmr        f1,f1
		0001985C D0210070 stfs       f1,0x70(r1)                   10 (00019858) REG
		00019860 3C600007 lis        r3,0x7
		00019864 C02329E4 lfs        f1,0x29E4(r3)                 04 (00019860) REG LSU
		00019868 FC200890 fmr        f1,f1
		0001986C D0210074 stfs       f1,0x74(r1)                   10 (00019868) REG
		00019870 3C600007 lis        r3,0x7
		00019874 C02329E8 lfs        f1,0x29E8(r3)                 04 (00019870) REG LSU
		00019878 FC200890 fmr        f1,f1
		0001987C D0210078 stfs       f1,0x78(r1)                   10 (00019878) REG
		float maxX = -FLT_MAX, maxY = -FLT_MAX, maxZ = -FLT_MAX;
	00019880 3C600007 lis        r3,0x7
		00019884 C02329EC lfs        f1,0x29EC(r3)                 04 (00019880) REG LSU
		00019888 FC200890 fmr        f1,f1
		0001988C D021007C stfs       f1,0x7C(r1)                   10 (00019888) REG
		00019890 3C600007 lis        r3,0x7
		00019894 C02329F0 lfs        f1,0x29F0(r3)                 04 (00019890) REG LSU
		00019898 FC200890 fmr        f1,f1
		0001989C D0210080 stfs       f1,0x80(r1)                   10 (00019898) REG
		000198A0 3C600007 lis        r3,0x7
		000198A4 C02329F4 lfs        f1,0x29F4(r3)                 04 (000198A0) REG LSU
		000198A8 FC200890 fmr        f1,f1
		000198AC D0210084 stfs       f1,0x84(r1)                   10 (000198A8) REG

		int32_t vertexStride = mesh.vertexStride;
	000198B0 3C600000 lis        r3,0x0
		000198B4 306300EC addic      r3,r3,0xEC                    01 (000198B0) REG PIPE
		000198B8 7C61182E lwzx       r3,r1,r3                      03 (000198B4) REG LSU
		000198BC 80630040 lwz        r3,0x40(r3)                   03 (000198B8) REG PIPE LSU
		000198C0 90610088 stw        r3,0x88(r1)                   03 (000198BC) REG LSU
		float* positions = (float*)( (uint8_t*)mesh.vertexData + mesh.vertexAttributes[VertexAttributes::kPosition].offset );
	000198C4 3C600000 lis        r3,0x0                         PIPE
		000198C8 306300EC addic      r3,r3,0xEC                    01 (000198C4) REG
		000198CC 7C61182E lwzx       r3,r1,r3                      04 (000198C8) REG LSU
		000198D0 80630050 lwz        r3,0x50(r3)                   03 (000198CC) REG LSU
		000198D4 3C800000 lis        r4,0x0
		000198D8 308400EC addic      r4,r4,0xEC                    01 (000198D4) REG
		000198DC 7C81202E lwzx       r4,r1,r4                      04 (000198D8) REG LSU
		000198E0 30840004 addic      r4,r4,0x4                     01 (000198DC) REG
		000198E4 78840020 clrldi     r4,r4,32                      01 (000198E0) REG PIPE
		000198E8 88840001 lbz        r4,0x1(r4)                    03 (000198E4) REG LSU
		000198EC 7C8407B4 extsw      r4,r4                         02 (000198E8) REG
		000198F0 7C632014 addc       r3,r3,r4                      01 (000198EC) REG
		000198F4 9061008C stw        r3,0x8C(r1)                   03 (000198F0) REG PIPE LSU
		EFW_ASSERT(vertexStride % sizeof(float) == 0);
	000198F8 80610088 lwz        r3,0x88(r1)
		000198FC 78630020 clrldi     r3,r3,32                      02 (000198F8) REG
		00019900 546307BE clrlwi     r3,r3,30                      01 (000198FC) REG
		00019904 7C6307B4 extsw      r3,r3                         01 (00019900) REG PIPE
		00019908 2C030000 cmpwi      r3,0x0                        01 (00019904) REG
		0001990C 40820008 bne        0x00019914                    01 (00019908) REG
		00019910 48000018 b          0x00019928                    08
		00019914 3C600007 lis        r3,0x7                         PIPE
		00019918 30632A1C addic      r3,r3,0x2A1C                  01 (00019914) REG
		0001991C 38800000 li         r4,0x0                         PIPE
		00019920 48036B7D bl         _SCE_Assert                   08
		00019924 60000000 nop                                       PIPE

		EFW_ASSERT(mesh.vertexAttributes[VertexAttributes::kPosition].componentCount == 3);
	00019928 3C600000 lis        r3,0x0
		0001992C 306300EC addic      r3,r3,0xEC                    01 (00019928) REG PIPE
		00019930 7C61182E lwzx       r3,r1,r3                      03 (0001992C) REG LSU
		00019934 30630004 addic      r3,r3,0x4                     02 (00019930) REG
		00019938 78630020 clrldi     r3,r3,32                      01 (00019934) REG
		0001993C 88630000 lbz        r3,0x0(r3)                    04 (00019938) REG LSU
		00019940 7C6307B4 extsw      r3,r3                         01 (0001993C) REG
		00019944 2C030003 cmpwi      r3,0x3                        01 (00019940) REG PIPE
		00019948 40820008 bne        0x00019950
		0001994C 48000018 b          0x00019964                    08 PIPE
		00019950 3C600007 lis        r3,0x7
		00019954 30632A70 addic      r3,r3,0x2A70                  01 (00019950) REG PIPE
		00019958 38800000 li         r4,0x0
		0001995C 48036B41 bl         _SCE_Assert                   08
		00019960 60000000 nop
		for (int32_t i=0; i<mesh.vertexCount; ++i)
			00019964 38600000 li         r3,0x0                         PIPE
			00019968 90610094 stw        r3,0x94(r1)                   03 (00019964) REG LSU
			0001996C 80610094 lwz        r3,0x94(r1)                   50 (00019968) PIPE LHS[01]
		00019970 3C800000 lis        r4,0x0
			00019974 308400EC addic      r4,r4,0xEC                    01 (00019970) REG PIPE
			00019978 7C81202E lwzx       r4,r1,r4                      03 (00019974) REG LSU
			0001997C 80840048 lwz        r4,0x48(r4)                   03 (00019978) REG PIPE LSU
			00019980 7C032000 cmpw       r3,r4                         01 (0001997C) REG
			00019984 40800128 bge        0x00019AAC                    01 (00019980) REG
		{
			if (positions[0] > maxX) maxX = positions[0];
			00019988 8061008C lwz        r3,0x8C(r1)
				0001998C C0230000 lfs        f1,0x0(r3)                    03 (00019988) REG PIPE LSU
				00019990 C041007C lfs        f2,0x7C(r1)
				00019994 FC011000 fcmpu      f1,f2                          PIPE
				00019998 41810008 bgt        0x000199A0                    27 (00019994) REG FLUSH
				0001999C 48000010 b          0x000199AC                    08 PIPE
				000199A0 8061008C lwz        r3,0x8C(r1)
				000199A4 C0230000 lfs        f1,0x0(r3)                    03 (000199A0) REG PIPE LSU
				000199A8 D021007C stfs       f1,0x7C(r1)
				if (positions[1] > maxY) maxY = positions[1];
			000199AC 8061008C lwz        r3,0x8C(r1)                    PIPE
				000199B0 C0230004 lfs        f1,0x4(r3)                    03 (000199AC) REG LSU
				000199B4 C0410080 lfs        f2,0x80(r1)                    PIPE
				000199B8 FC011000 fcmpu      f1,f2
				000199BC 41810008 bgt        0x000199C4                    27 (000199B8) REG FLUSH
				000199C0 48000010 b          0x000199D0                    08
				000199C4 8061008C lwz        r3,0x8C(r1)                    PIPE
				000199C8 C0230004 lfs        f1,0x4(r3)                    03 (000199C4) REG LSU
				000199CC D0210080 stfs       f1,0x80(r1)                    PIPE
				if (positions[2] > maxZ) maxZ = positions[2];
			000199D0 8061008C lwz        r3,0x8C(r1)
				000199D4 C0230008 lfs        f1,0x8(r3)                    03 (000199D0) REG PIPE LSU
				000199D8 C0410084 lfs        f2,0x84(r1)
				000199DC FC011000 fcmpu      f1,f2                          PIPE
				000199E0 41810008 bgt        0x000199E8                    27 (000199DC) REG FLUSH
				000199E4 48000010 b          0x000199F4                    08 PIPE
				000199E8 8061008C lwz        r3,0x8C(r1)
				000199EC C0230008 lfs        f1,0x8(r3)                    03 (000199E8) REG PIPE LSU
				000199F0 D0210084 stfs       f1,0x84(r1)
				if (positions[0] < minX) minX = positions[0];
			000199F4 8061008C lwz        r3,0x8C(r1)                    PIPE
				000199F8 C0230000 lfs        f1,0x0(r3)                    03 (000199F4) REG LSU
				000199FC C0410070 lfs        f2,0x70(r1)                    PIPE
				00019A00 FC011000 fcmpu      f1,f2
				00019A04 41800008 blt        0x00019A0C                    27 (00019A00) REG FLUSH
				00019A08 48000010 b          0x00019A18                    08
				00019A0C 8061008C lwz        r3,0x8C(r1)                    PIPE
				00019A10 C0230000 lfs        f1,0x0(r3)                    03 (00019A0C) REG LSU
				00019A14 D0210070 stfs       f1,0x70(r1)                    PIPE
				if (positions[1] < minY) minY = positions[1];
			00019A18 8061008C lwz        r3,0x8C(r1)
				00019A1C C0230004 lfs        f1,0x4(r3)                    03 (00019A18) REG PIPE LSU
				00019A20 C0410074 lfs        f2,0x74(r1)
				00019A24 FC011000 fcmpu      f1,f2                          PIPE
				00019A28 41800008 blt        0x00019A30                    27 (00019A24) REG FLUSH
				00019A2C 48000010 b          0x00019A3C                    08 PIPE
				00019A30 8061008C lwz        r3,0x8C(r1)
				00019A34 C0230004 lfs        f1,0x4(r3)                    03 (00019A30) REG PIPE LSU
				00019A38 D0210074 stfs       f1,0x74(r1)
				if (positions[2] < minZ) minZ = positions[2];
			00019A3C 8061008C lwz        r3,0x8C(r1)                    PIPE
				00019A40 C0230008 lfs        f1,0x8(r3)                    03 (00019A3C) REG LSU
				00019A44 C0410078 lfs        f2,0x78(r1)                    PIPE
				00019A48 FC011000 fcmpu      f1,f2
				00019A4C 41800008 blt        0x00019A54                    27 (00019A48) REG FLUSH
				00019A50 48000010 b          0x00019A60                    08
				00019A54 8061008C lwz        r3,0x8C(r1)                    PIPE
				00019A58 C0230008 lfs        f1,0x8(r3)                    03 (00019A54) REG LSU
				00019A5C D0210078 stfs       f1,0x78(r1)                    PIPE
				positions += vertexStride/sizeof(float);
			00019A60 8061008C lwz        r3,0x8C(r1)
				00019A64 80810088 lwz        r4,0x88(r1)                    PIPE
				00019A68 78840020 clrldi     r4,r4,32                      01 (00019A64) REG
				00019A6C 5484F0BE srwi       r4,r4,2                       01 (00019A68) REG PIPE
				00019A70 7C8407B4 extsw      r4,r4                         01 (00019A6C) REG
				00019A74 78841764 sldi       r4,r4,2                       01 (00019A70) REG PIPE
				00019A78 78840020 clrldi     r4,r4,32                      01 (00019A74) REG
				00019A7C 7C632014 addc       r3,r3,r4                      01 (00019A78) REG PIPE
				00019A80 9061008C stw        r3,0x8C(r1)                   03 (00019A7C) REG LSU
				for (int32_t i=0; i<mesh.vertexCount; ++i)
					00019A84 80610094 lwz        r3,0x94(r1)                    PIPE
					00019A88 30630001 addic      r3,r3,0x1                     01 (00019A84) REG
					00019A8C 90610094 stw        r3,0x94(r1)                   03 (00019A88) REG PIPE LSU
					00019A90 80610094 lwz        r3,0x94(r1)                   50 (00019A8C) LHS[01]
				00019A94 3C800000 lis        r4,0x0                         PIPE
					00019A98 308400EC addic      r4,r4,0xEC                    01 (00019A94) REG
					00019A9C 7C81202E lwzx       r4,r1,r4                      04 (00019A98) REG LSU
					00019AA0 80840048 lwz        r4,0x48(r4)                   03 (00019A9C) REG LSU
					00019AA4 7C032000 cmpw       r3,r4                         02 (00019AA0) REG
					00019AA8 4180FEE0 blt        0x00019988
		}
		outBoudingSphere[0] = (minX+maxX) * 0.5f;
		00019AAC C021007C lfs        f1,0x7C(r1)
			00019AB0 C0410070 lfs        f2,0x70(r1)
			00019AB4 EC21102A fadds      f1,f1,f2                       PIPE
			00019AB8 3C600007 lis        r3,0x7
			00019ABC C04329F8 lfs        f2,0x29F8(r3)                 04 (00019AB8) REG LSU
			00019AC0 FC401090 fmr        f2,f2
			00019AC4 EC2100B2 fmuls      f1,f1,f2                      09 (00019AC0) REG PIPE
			00019AC8 3C600000 lis        r3,0x0
			00019ACC 306300E4 addic      r3,r3,0xE4                    01 (00019AC8) REG PIPE
			00019AD0 7C61182E lwzx       r3,r1,r3                      03 (00019ACC) REG LSU
			00019AD4 D0230000 stfs       f1,0x0(r3)                    03 (00019AD0) REG PIPE LSU
			outBoudingSphere[1] = (minY+maxY) * 0.5f;
		00019AD8 C0210080 lfs        f1,0x80(r1)
			00019ADC C0410074 lfs        f2,0x74(r1)                    PIPE
			00019AE0 EC21102A fadds      f1,f1,f2
			00019AE4 3C600007 lis        r3,0x7
			00019AE8 C04329FC lfs        f2,0x29FC(r3)                 03 (00019AE4) REG LSU
			00019AEC FC401090 fmr        f2,f2                          PIPE
			00019AF0 EC2100B2 fmuls      f1,f1,f2                      09 (00019AEC) REG
			00019AF4 3C600000 lis        r3,0x0
			00019AF8 306300E4 addic      r3,r3,0xE4                    01 (00019AF4) REG
			00019AFC 7C61182E lwzx       r3,r1,r3                      04 (00019AF8) REG LSU
			00019B00 D0230004 stfs       f1,0x4(r3)                    03 (00019AFC) REG LSU
			outBoudingSphere[2] = (minZ+maxZ) * 0.5f;
		00019B04 C0210084 lfs        f1,0x84(r1)                    PIPE
			00019B08 C0410078 lfs        f2,0x78(r1)
			00019B0C EC21102A fadds      f1,f1,f2                       PIPE
			00019B10 3C600007 lis        r3,0x7
			00019B14 C0432A00 lfs        f2,0x2A00(r3)                 04 (00019B10) REG LSU
			00019B18 FC401090 fmr        f2,f2
			00019B1C EC2100B2 fmuls      f1,f1,f2                      09 (00019B18) REG PIPE
			00019B20 3C600000 lis        r3,0x0
			00019B24 306300E4 addic      r3,r3,0xE4                    01 (00019B20) REG PIPE
			00019B28 7C61182E lwzx       r3,r1,r3                      03 (00019B24) REG LSU
			00019B2C D0230008 stfs       f1,0x8(r3)                    03 (00019B28) REG PIPE LSU

			float distanceSquared = 0;
		00019B30 3C600007 lis        r3,0x7
			00019B34 C0232A04 lfs        f1,0x2A04(r3)                 04 (00019B30) REG LSU
			00019B38 FC200890 fmr        f1,f1
			00019B3C D0210090 stfs       f1,0x90(r1)                   10 (00019B38) REG
			positions = (float*)( (uint8_t*)mesh.vertexData + mesh.vertexAttributes[VertexAttributes::kPosition].offset );
		00019B40 3C600000 lis        r3,0x0
			00019B44 306300EC addic      r3,r3,0xEC                    01 (00019B40) REG PIPE
			00019B48 7C61182E lwzx       r3,r1,r3                      03 (00019B44) REG LSU
			00019B4C 80630050 lwz        r3,0x50(r3)                   03 (00019B48) REG PIPE LSU
			00019B50 3C800000 lis        r4,0x0
			00019B54 308400EC addic      r4,r4,0xEC                    01 (00019B50) REG PIPE
			00019B58 7C81202E lwzx       r4,r1,r4                      03 (00019B54) REG LSU
			00019B5C 30840004 addic      r4,r4,0x4                     02 (00019B58) REG
			00019B60 78840020 clrldi     r4,r4,32                      01 (00019B5C) REG
			00019B64 88840001 lbz        r4,0x1(r4)                    04 (00019B60) REG LSU
			00019B68 7C8407B4 extsw      r4,r4                         01 (00019B64) REG
			00019B6C 7C632014 addc       r3,r3,r4                      01 (00019B68) REG PIPE
			00019B70 9061008C stw        r3,0x8C(r1)                   03 (00019B6C) REG LSU
			for (int32_t i=0; i<mesh.vertexCount; ++i)
				00019B74 38600000 li         r3,0x0                         PIPE
				00019B78 90610094 stw        r3,0x94(r1)                   03 (00019B74) REG LSU
				00019B7C 80610094 lwz        r3,0x94(r1)                   50 (00019B78) PIPE LHS[01]
			00019B80 3C800000 lis        r4,0x0
				00019B84 308400EC addic      r4,r4,0xEC                    01 (00019B80) REG PIPE
				00019B88 7C81202E lwzx       r4,r1,r4                      03 (00019B84) REG LSU
				00019B8C 80840048 lwz        r4,0x48(r4)                   03 (00019B88) REG PIPE LSU
				00019B90 7C032000 cmpw       r3,r4                         01 (00019B8C) REG
				00019B94 40800130 bge        0x00019CC4                    01 (00019B90) REG
			{
				float distX = positions[0] - outBoudingSphere[0];
				00019B98 8061008C lwz        r3,0x8C(r1)
					00019B9C C0230000 lfs        f1,0x0(r3)                    03 (00019B98) REG PIPE LSU
					00019BA0 3C600000 lis        r3,0x0
					00019BA4 306300E4 addic      r3,r3,0xE4                    01 (00019BA0) REG PIPE
					00019BA8 7C61182E lwzx       r3,r1,r3                      03 (00019BA4) REG LSU
					00019BAC C0430000 lfs        f2,0x0(r3)                    03 (00019BA8) REG PIPE LSU
					00019BB0 EC211028 fsubs      f1,f1,f2
					00019BB4 D0210098 stfs       f1,0x98(r1)                   10 (00019BB0) REG
					float distY = positions[1] - outBoudingSphere[1];
				00019BB8 8061008C lwz        r3,0x8C(r1)
					00019BBC C0230004 lfs        f1,0x4(r3)                    03 (00019BB8) REG PIPE LSU
					00019BC0 3C600000 lis        r3,0x0
					00019BC4 306300E4 addic      r3,r3,0xE4                    01 (00019BC0) REG PIPE
					00019BC8 7C61182E lwzx       r3,r1,r3                      03 (00019BC4) REG LSU
					00019BCC C0430004 lfs        f2,0x4(r3)                    03 (00019BC8) REG PIPE LSU
					00019BD0 EC211028 fsubs      f1,f1,f2
					00019BD4 D021009C stfs       f1,0x9C(r1)                   10 (00019BD0) REG
					float distZ = positions[2] - outBoudingSphere[2];
				00019BD8 8061008C lwz        r3,0x8C(r1)
					00019BDC C0230008 lfs        f1,0x8(r3)                    03 (00019BD8) REG PIPE LSU
					00019BE0 3C600000 lis        r3,0x0
					00019BE4 306300E4 addic      r3,r3,0xE4                    01 (00019BE0) REG PIPE
					00019BE8 7C61182E lwzx       r3,r1,r3                      03 (00019BE4) REG LSU
					00019BEC C0430008 lfs        f2,0x8(r3)                    03 (00019BE8) REG PIPE LSU
					00019BF0 EC211028 fsubs      f1,f1,f2
					00019BF4 D02100A0 stfs       f1,0xA0(r1)                   10 (00019BF0) REG
					distanceSquared = MAX(distanceSquared, distX*distX+distY*distY+distZ*distZ);
				00019BF8 C0210090 lfs        f1,0x90(r1)
					00019BFC C0410098 lfs        f2,0x98(r1)                    PIPE
					00019C00 C0410098 lfs        f2,0x98(r1)
					00019C04 EC4200B2 fmuls      f2,f2,f2                       PIPE
					00019C08 C061009C lfs        f3,0x9C(r1)
					00019C0C C061009C lfs        f3,0x9C(r1)                    PIPE
					00019C10 EC6300F2 fmuls      f3,f3,f3
					00019C14 EC42182A fadds      f2,f2,f3                      09 (00019C10) REG PIPE
					00019C18 C06100A0 lfs        f3,0xA0(r1)                   70 (00019BF4) LHS[18]
				00019C1C C06100A0 lfs        f3,0xA0(r1)                    PIPE
					00019C20 EC6300F2 fmuls      f3,f3,f3
					00019C24 EC42182A fadds      f2,f2,f3                      09 (00019C20) REG PIPE
					00019C28 FC011000 fcmpu      f1,f2                         09 (00019C24) REG
					00019C2C 41810008 bgt        0x00019C34                    27 (00019C28) REG FLUSH
					00019C30 48000010 b          0x00019C40                    08
					00019C34 C0210090 lfs        f1,0x90(r1)                    PIPE
					00019C38 D02100A4 stfs       f1,0xA4(r1)
					00019C3C 48000034 b          0x00019C70                    08
					00019C40 C0210098 lfs        f1,0x98(r1)
					00019C44 C0210098 lfs        f1,0x98(r1)                    PIPE
					00019C48 EC210072 fmuls      f1,f1,f1
					00019C4C C041009C lfs        f2,0x9C(r1)
					00019C50 C041009C lfs        f2,0x9C(r1)
					00019C54 EC4200B2 fmuls      f2,f2,f2                       PIPE
					00019C58 EC21102A fadds      f1,f1,f2                      09 (00019C54) REG
					00019C5C C04100A0 lfs        f2,0xA0(r1)
					00019C60 C04100A0 lfs        f2,0xA0(r1)
					00019C64 EC4200B2 fmuls      f2,f2,f2                       PIPE
					00019C68 EC21102A fadds      f1,f1,f2                      09 (00019C64) REG
					00019C6C D02100A4 stfs       f1,0xA4(r1)                   10 (00019C68) REG
					00019C70 C02100A4 lfs        f1,0xA4(r1)                   70 (00019C6C) LHS[01]
				00019C74 D0210090 stfs       f1,0x90(r1)                    PIPE
					positions += vertexStride/sizeof(float);
				00019C78 8061008C lwz        r3,0x8C(r1)
					00019C7C 80810088 lwz        r4,0x88(r1)                    PIPE
					00019C80 78840020 clrldi     r4,r4,32                      01 (00019C7C) REG
					00019C84 5484F0BE srwi       r4,r4,2                       01 (00019C80) REG PIPE
					00019C88 7C8407B4 extsw      r4,r4                         01 (00019C84) REG
					00019C8C 78841764 sldi       r4,r4,2                       01 (00019C88) REG PIPE
					00019C90 78840020 clrldi     r4,r4,32                      01 (00019C8C) REG
					00019C94 7C632014 addc       r3,r3,r4                      01 (00019C90) REG PIPE
					00019C98 9061008C stw        r3,0x8C(r1)                   03 (00019C94) REG LSU
					for (int32_t i=0; i<mesh.vertexCount; ++i)
						00019C9C 80610094 lwz        r3,0x94(r1)                    PIPE
						00019CA0 30630001 addic      r3,r3,0x1                     01 (00019C9C) REG
						00019CA4 90610094 stw        r3,0x94(r1)                   03 (00019CA0) REG PIPE LSU
						00019CA8 80610094 lwz        r3,0x94(r1)                   50 (00019CA4) LHS[01]
					00019CAC 3C800000 lis        r4,0x0                         PIPE
						00019CB0 308400EC addic      r4,r4,0xEC                    01 (00019CAC) REG
						00019CB4 7C81202E lwzx       r4,r1,r4                      04 (00019CB0) REG LSU
						00019CB8 80840048 lwz        r4,0x48(r4)                   03 (00019CB4) REG LSU
						00019CBC 7C032000 cmpw       r3,r4                         02 (00019CB8) REG
						00019CC0 4180FED8 blt        0x00019B98
			}

			// Calculate bounding sphere radius
			outBoudingSphere[3] = sqrtf(distanceSquared);
			00019CC4 C0210090 lfs        f1,0x90(r1)
				00019CC8 EC20082C fsqrts     f1,f1                         56
				00019CCC D02100A8 stfs       f1,0xA8(r1)                    PIPE
				00019CD0 FC010800 fcmpu      f1,f1
				00019CD4 40820010 bne        0x00019CE4                    27 (00019CD0) REG FLUSH
				00019CD8 C02100A8 lfs        f1,0xA8(r1)
				00019CDC D02100AC stfs       f1,0xAC(r1)                    PIPE
				00019CE0 48000010 b          0x00019CF0                    08
				00019CE4 C0210090 lfs        f1,0x90(r1)                    PIPE
				00019CE8 EC20082C fsqrts     f1,f1                         56
				00019CEC D02100AC stfs       f1,0xAC(r1)                    PIPE
				00019CF0 3C600000 lis        r3,0x0
				00019CF4 306300E4 addic      r3,r3,0xE4                    01 (00019CF0) REG PIPE
				00019CF8 7C61182E lwzx       r3,r1,r3                      03 (00019CF4) REG LSU
				00019CFC C02100AC lfs        f1,0xAC(r1)                   70 (00019CEC) PIPE LHS[08]
			00019D00 D023000C stfs       f1,0xC(r3)

				EFW_ASSERT(outBoudingSphere[0] == outBoudingSphere[0] &&
				00019D04 3C600000 lis        r3,0x0
				00019D08 306300E4 addic      r3,r3,0xE4                    01 (00019D04) REG
				00019D0C 7C61182E lwzx       r3,r1,r3                      04 (00019D08) REG LSU
				00019D10 C0230000 lfs        f1,0x0(r3)                    03 (00019D0C) REG LSU
				00019D14 3C600000 lis        r3,0x0
				00019D18 306300E4 addic      r3,r3,0xE4                    01 (00019D14) REG
				00019D1C 7C61182E lwzx       r3,r1,r3                      04 (00019D18) REG LSU
				00019D20 C0230000 lfs        f1,0x0(r3)                    03 (00019D1C) REG LSU
				00019D24 FC010800 fcmpu      f1,f1                          PIPE
				00019D28 40820080 bne        0x00019DA8                    27 (00019D24) REG FLUSH
				00019D2C 3C600000 lis        r3,0x0
				00019D30 306300E4 addic      r3,r3,0xE4                    01 (00019D2C) REG
				00019D34 7C61182E lwzx       r3,r1,r3                      04 (00019D30) REG LSU
				00019D38 C0230004 lfs        f1,0x4(r3)                    03 (00019D34) REG LSU
				00019D3C 3C600000 lis        r3,0x0
				00019D40 306300E4 addic      r3,r3,0xE4                    01 (00019D3C) REG
				00019D44 7C61182E lwzx       r3,r1,r3                      04 (00019D40) REG LSU
				00019D48 C0230004 lfs        f1,0x4(r3)                    03 (00019D44) REG LSU
				00019D4C FC010800 fcmpu      f1,f1                          PIPE
				00019D50 40820058 bne        0x00019DA8                    27 (00019D4C) REG FLUSH
				00019D54 3C600000 lis        r3,0x0
				00019D58 306300E4 addic      r3,r3,0xE4                    01 (00019D54) REG
				00019D5C 7C61182E lwzx       r3,r1,r3                      04 (00019D58) REG LSU
				00019D60 C0230008 lfs        f1,0x8(r3)                    03 (00019D5C) REG LSU
				00019D64 3C600000 lis        r3,0x0
				00019D68 306300E4 addic      r3,r3,0xE4                    01 (00019D64) REG
				00019D6C 7C61182E lwzx       r3,r1,r3                      04 (00019D68) REG LSU
				00019D70 C0230008 lfs        f1,0x8(r3)                    03 (00019D6C) REG LSU
				00019D74 FC010800 fcmpu      f1,f1                          PIPE
				00019D78 40820030 bne        0x00019DA8                    27 (00019D74) REG FLUSH
				00019D7C 3C600000 lis        r3,0x0
				00019D80 306300E4 addic      r3,r3,0xE4                    01 (00019D7C) REG
				00019D84 7C61182E lwzx       r3,r1,r3                      04 (00019D80) REG LSU
				00019D88 C023000C lfs        f1,0xC(r3)                    03 (00019D84) REG LSU
				00019D8C 3C600000 lis        r3,0x0
				00019D90 306300E4 addic      r3,r3,0xE4                    01 (00019D8C) REG
				00019D94 7C61182E lwzx       r3,r1,r3                      04 (00019D90) REG LSU
				00019D98 C023000C lfs        f1,0xC(r3)                    03 (00019D94) REG LSU
				00019D9C FC010800 fcmpu      f1,f1                          PIPE
				00019DA0 40820008 bne        0x00019DA8                    27 (00019D9C) REG FLUSH
				00019DA4 48000018 b          0x00019DBC                    08 PIPE
				00019DA8 3C600007 lis        r3,0x7
				00019DAC 30632AE8 addic      r3,r3,0x2AE8                  01 (00019DA8) REG PIPE
				00019DB0 38800000 li         r4,0x0
				00019DB4 480366E9 bl         _SCE_Assert                   08
				00019DB8 60000000 nop
				outBoudingSphere[1] == outBoudingSphere[1] &&
				outBoudingSphere[2] == outBoudingSphere[2] &&
				outBoudingSphere[3] == outBoudingSphere[3]);
			EFW_ASSERT(outBoudingSphere[3] != FLT_MAX);
			00019DBC 3C600000 lis        r3,0x0                         PIPE
				00019DC0 306300E4 addic      r3,r3,0xE4                    01 (00019DBC) REG
				00019DC4 7C61182E lwzx       r3,r1,r3                      04 (00019DC0) REG LSU
				00019DC8 C023000C lfs        f1,0xC(r3)                    03 (00019DC4) REG LSU
				00019DCC 3C600007 lis        r3,0x7
				00019DD0 C0432A08 lfs        f2,0x2A08(r3)                 03 (00019DCC) REG LSU
				00019DD4 FC401090 fmr        f2,f2                          PIPE
				00019DD8 FC011000 fcmpu      f1,f2                         09 (00019DD4) REG
				00019DDC 41820008 beq        0x00019DE4                    27 (00019DD8) REG FLUSH
				00019DE0 48000018 b          0x00019DF8                    08
				00019DE4 3C600007 lis        r3,0x7                         PIPE
				00019DE8 30632BD0 addic      r3,r3,0x2BD0                  01 (00019DE4) REG
				00019DEC 38800000 li         r4,0x0                         PIPE
				00019DF0 480366AD bl         _SCE_Assert                   08
				00019DF4 60000000 nop                                       PIPE
}
00019DF8 E80100C0 ld         r0,0xC0(r1)
	00019DFC 7C0803A6 mtspr      lr,r0                         02 (00019DF8) REG
	00019E00 382100B0 addi       r1,r1,0xB0
*/
