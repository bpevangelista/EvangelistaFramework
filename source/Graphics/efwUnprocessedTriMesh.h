/**
 * Mesh intermediate format
 * @author Bruno Evangelista
 *
 * THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */
#pragma once

#include "Foundation/efwPlatform.h"

namespace efw
{

namespace Graphics
{
	const int32_t kObjTriMeshMaxNameLength = 64;

	struct UnprocessedMaterialTexture
	{
		int32_t width;
		int32_t height;
		int32_t pitch;
		int32_t format;
		void* data;

		char sourceFilename[Path::kMaxFullPathLength];
	};

	struct UnprocessedMaterial
	{
		int64_t id;

		UnprocessedMaterialTexture* albedoTexture;
		UnprocessedMaterialTexture* normalMapTexture;
		//UnprocessedMaterialTexture specularTexture;
		//UnprocessedMaterialTexture specularPowerTexture;
	};


	struct UnprocessedMaterialLib
	{
		int32_t materialCount;
		UnprocessedMaterial materials[];
	};

	namespace VertexAttributes
	{
		const int32_t kPosition		= 0;
		const int32_t kNormal		= 1;
		const int32_t kTangent		= 2;
		const int32_t kBinormal		= 3;
		const int32_t kColor		= 4;
		const int32_t kBlendIndex	= 5;
		const int32_t kBlendWeight	= 6;
		const int32_t kUv0			= 7;
		const int32_t kUv1			= 8;
		const int32_t kUv2			= 9;
		const int32_t kUv3			= 10;
		const int32_t kUv4			= 11;
		const int32_t kUv5			= 12;
		const int32_t kUv6			= 13;
		const int32_t kUv7			= 14;
		const int32_t kCount		= 15;
	}

	struct UnprocessedTriMeshVertexAttribute
	{
		uint8_t componentCount;					// Number of 32b components (0 if data is not present)
		uint8_t offset;							// Offset from start of vertex array in 32b components
		uint8_t _pad[2];
	};

	//struct UnprocessedExternalResource
	//{
	//	int32_t resourceNameLength;
	//	char* resourceName;
	//};

	/**
	 * Intermediate format used to store unprocessed triangle meshes coming from any input format. 
	 * Meshes are stored as indexed triangle lists, vertexes as a single interleaved array and indices as a separated array.
	 * Vertex attributes are stored as 32b per-component (e.g. 128b  for XYZW), and indexes as 32b per-index.
	 */
	struct UnprocessedTriMesh
	{
		int32_t materialId;

		UnprocessedTriMeshVertexAttribute vertexAttributes[VertexAttributes::kCount];
		int32_t vertexStride;
		int32_t indexStride;
		int32_t vertexCount;
		int32_t indexCount;
		void* vertexData;
		void* indexData;

		// Debug
		char name[kObjTriMeshMaxNameLength];
	};

	/**
	 * Intermediate format used to store unprocessed triangle models.
	 */
	struct UnprocessedTriModel
	{
		int32_t meshCount;
		UnprocessedTriMesh meshes[];
	};
}
}