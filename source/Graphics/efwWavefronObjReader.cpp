#include "Graphics/efwWavefrontObjReader.h"

#include <vector>
#include <map>

#include "Foundation/efwMemory.h"

#include "Foundation/efwConsole.h"
#include "Foundation/efwFileReader.h"
#include "Foundation/efwPathHelper.h"
#include "Foundation/efwStringHelper.h"
#include "Graphics/efwTextureReader.h"
#include "Graphics/efwUnprocessedTriMesh.h"


using namespace std;
using namespace efw;
using namespace efw::Graphics;

struct WavefrontObjVertexAttribute
{
	float x, y, z;
};

struct WavefrontObjVertexAttributes
{
	vector<WavefrontObjVertexAttribute> positions;
	vector<WavefrontObjVertexAttribute> normals;
	vector<WavefrontObjVertexAttribute> uvs;
};


// Global line counter
int32_t gModelInputLineCount = 0;
int32_t gMaterialInputLineCount = 0;


int32_t ParseVertexAttribute(WavefrontObjVertexAttributes* outVertexAttributes, const TokenArray* tokenArray)
{
	if (tokenArray->count < 4)
	{
		//Console::WriteLine("Line %06d: [SKIPPED] Invalid or not supported data!", gModelInputLineCount);
		return efwErrs::kInvalidInput;
	}
	else if (tokenArray->count > 4)
	{
		Console::WriteLine("Line %06d: Too many tokens in line! Part of the data in being discarded!", gModelInputLineCount);
	}

	const char* token = StringHelper::GetTokenAt(tokenArray, 0);

	vector<WavefrontObjVertexAttribute>* pushArray = NULL;
	if (token[0] == 'v')
	{
		switch(token[1])
		{
		case '\0':
			pushArray = &outVertexAttributes->positions;
			break;
		case 't':
			pushArray = &outVertexAttributes->uvs;
			break;
		case 'n':
			pushArray = &outVertexAttributes->normals;
			break;

			// Not handled
		default:
			//Console::WriteLine("Line %06d: [SKIPPED] Invalid or not supported data!", gModelInputLineCount);
			return efwErrs::kInvalidInput;
		}
	}

	WavefrontObjVertexAttribute attr;
	attr.x = (float)atof( StringHelper::GetTokenAt(tokenArray, 1) );
	attr.y = (float)atof( StringHelper::GetTokenAt(tokenArray, 2) );
	attr.z = (float)atof( StringHelper::GetTokenAt(tokenArray, 3) );
	pushArray->push_back( attr );

	return efwErrs::kOk;
}


int32_t ParseFace(vector<int32_t>* outIndexData, vector<uint64_t>* currentVertexIndexToAttributes, 
	map<uint64_t, int32_t>* currentVertexAttributesToIndex, const WavefrontObjVertexAttributes& vertexAttributes, const TokenArray* tokenArray)
{
	// Only supports faces with 3 or 4 vertices
	if (tokenArray->count < 4 || tokenArray->count > 5)
	{
		//Console::WriteLine("Line %06d: [SKIPPED] \"%d\" is not supported as the number of faces!", gModelInputLineCount, tokenArray->count);
		return efwErrs::kInvalidInput;
	}

	// Get the position, texture and normal tokens for each face index
	const int32_t kMaxVerticesPerFace = 4;
	const int32_t kMaxAttributesPerVertex = 3;
	TokenArray* faceVertexAttributes;
	StringHelper::CreateTokenArray(&faceVertexAttributes, sizeof(Token32), kMaxAttributesPerVertex);

	int32_t faceVertexIndices[kMaxVerticesPerFace];
	int32_t faceVertexCount = tokenArray->count - 1;
	for (int32_t i=0; i<faceVertexCount; i++)
	{
		const char* kObjTriMeshFaceDemiliters = "/";
		StringHelper::GetTokens(faceVertexAttributes, StringHelper::GetTokenAt(tokenArray, i+1), kObjTriMeshFaceDemiliters, true);
		if (faceVertexAttributes->count == 0)
		{
			// TODO: Error
			continue;
		}

		const int32_t kInvalidIndex = -1;
		const char* positionToken = StringHelper::GetTokenAt(faceVertexAttributes, 0);
		const char* uvToken = StringHelper::GetTokenAt(faceVertexAttributes, 1);
		const char* normalToken = StringHelper::GetTokenAt(faceVertexAttributes, 2);
		size_t positionIndex = atoi( positionToken ) - 1;
		size_t uvIndex = (faceVertexAttributes->count > 1 && uvToken[0] != '\0')? atoi(uvToken) - 1 : kInvalidIndex;
		size_t normalIndex = (faceVertexAttributes->count > 2 && normalToken[0] != '\0')? atoi(normalToken) - 1 : kInvalidIndex;

		// It's much better to validate the face parsing at this point than later
		const uint32_t kLastIndex = 0xFFFFFFFF;
		const uint32_t kVertexIndexMask = (1 << 21) - 1;
		bool positionOutOfBounds = !( positionIndex <= kVertexIndexMask && positionIndex < vertexAttributes.positions.size() );
		bool indexOutOfBounds = !( (uvIndex == kLastIndex) || (uvIndex <= kVertexIndexMask && uvIndex < vertexAttributes.uvs.size()) );
		bool normalOutOfBounds = !( (normalIndex == kLastIndex) || (normalIndex <= kVertexIndexMask && normalIndex < vertexAttributes.normals.size()) );
		if (positionOutOfBounds || indexOutOfBounds || normalOutOfBounds)
		{
			//Console::WriteLine("Line %06d: [SKIPPED] Indexing out of bounds attributes!", gModelInputLineCount);
			continue;
		}

		// Use 21bits for each vertex attribute index
		uint64_t vertexGuid = 
			((uint64_t)(uvIndex & kVertexIndexMask) << 42) | 
			((uint64_t)(normalIndex & kVertexIndexMask) << 21) | 
			(positionIndex & kVertexIndexMask);

		// 
		int32_t interleavedVertexIndex = kInvalidIndex;
		map<uint64_t, int32_t>::iterator it = currentVertexAttributesToIndex->find(vertexGuid);
		if (it == currentVertexAttributesToIndex->end())
		{
			interleavedVertexIndex = currentVertexAttributesToIndex->size();
			currentVertexAttributesToIndex->insert( pair<uint64_t,int32_t>(vertexGuid, interleavedVertexIndex) );
			currentVertexIndexToAttributes->push_back(vertexGuid);
		}
		else
		{
			interleavedVertexIndex = it->second;
		}

		faceVertexIndices[i] = interleavedVertexIndex;
	}

	// Append indices
	if (faceVertexCount == 3)
	{
		int32_t indices[] = { 2, 1, 0 };
		for (int32_t i=0; i<COUNTOF(indices); i++)
		{
			outIndexData->push_back( faceVertexIndices[ indices[i] ] );
		}
	}
	else if (faceVertexCount == 4)
	{
		int32_t indices[] = { 2, 1, 0, 2, 0, 3 };
		for (int32_t i=0; i<COUNTOF(indices); i++)
		{
			outIndexData->push_back( faceVertexIndices[ indices[i] ] );
		}
	}

	StringHelper::DestroyTokenArray(&faceVertexAttributes);

	return efwErrs::kOk;
}


int32_t FillMeshVertexAttributes(UnprocessedTriMesh* outMesh, const WavefrontObjVertexAttributes& vertexAttributes)
{
	int32_t vertexAttributeOffset = 0;
	const int32_t kVertexPositionComponents = 3;
	const int32_t kVertexNormalComponents = 3;
	const int32_t kVertexUvComponents = 2;

	// Process positions
	const bool hasPositions = (vertexAttributes.positions.size() > 0);
	if (!hasPositions)
	{
		//Console::WriteLine("Line %06d: [SKIPPED] Indexing out of bounds attributes!", gModelInputLineCount);
		return efwErrs::kInvalidInput;
	}

	outMesh->vertexAttributes[VertexAttributes::kPosition].offset = vertexAttributeOffset;
	outMesh->vertexAttributes[VertexAttributes::kPosition].componentCount = kVertexPositionComponents;
	vertexAttributeOffset += kVertexPositionComponents * sizeof(float);
	
	const bool hasNormals = (vertexAttributes.normals.size() > 0);
	if (hasNormals)
	{
		outMesh->vertexAttributes[VertexAttributes::kNormal].offset = vertexAttributeOffset;
		outMesh->vertexAttributes[VertexAttributes::kNormal].componentCount = kVertexNormalComponents;
		vertexAttributeOffset += kVertexNormalComponents * sizeof(float);
	}

	const bool hasUvs = (vertexAttributes.uvs.size() > 0);
	if (hasUvs)
	{
		outMesh->vertexAttributes[VertexAttributes::kUv0].offset = vertexAttributeOffset;
		outMesh->vertexAttributes[VertexAttributes::kUv0].componentCount = kVertexUvComponents;
		vertexAttributeOffset += kVertexUvComponents * sizeof(float);
	}
	outMesh->vertexStride = vertexAttributeOffset;

	return efwErrs::kOk;
}


int32_t GenerateMesh(UnprocessedTriMesh* outMesh, const WavefrontObjVertexAttributes& vertexAttributes, 
	const vector<uint64_t>& vertexList, const vector<int32_t>& indexData)
{
	if (outMesh == NULL)
	{
		// TODO
		return efwErrs::kInvalidInput;
	}

	if (strcmp(outMesh->name, "") == 0)
	{
		const char kDefaultMeshName[] = "NoName";
		memcpy(outMesh->name, kDefaultMeshName, COUNTOF(kDefaultMeshName));
	}

	// Process index data
	const int32_t indexDataCount = indexData.size();
	outMesh->indexStride = sizeof(int32_t);
	outMesh->indexCount = indexDataCount;
	outMesh->indexData = memalign(1024, outMesh->indexCount * outMesh->indexStride);
	memcpy((void*)outMesh->indexData, &indexData[0], indexDataCount * outMesh->indexStride);

	// Process vertex attributes
	FillMeshVertexAttributes(outMesh, vertexAttributes);
	const bool hasNormals = (vertexAttributes.normals.size() > 0);
	const bool hasUvs = (vertexAttributes.uvs.size() > 0);

	// Vertex data
	const int32_t vertexListCount = vertexList.size();
	outMesh->vertexCount = vertexListCount;
	outMesh->vertexData = memalign(1024, vertexListCount * outMesh->vertexStride);

	float* meshVertexData = (float*)outMesh->vertexData;
	int32_t dataIndex = 0;
	for (int32_t i=0; i<vertexListCount; ++i)
	{
		const uint32_t kIndexMask = (1 << 21) - 1;
		uint64_t vertexGuid = vertexList[i];
		int32_t positionIndex = vertexGuid & kIndexMask;
		int32_t normalIndex = (vertexGuid >> 21) & kIndexMask;
		int32_t uvIndex = (vertexGuid >> 42) & kIndexMask;

		meshVertexData[dataIndex++] = vertexAttributes.positions[positionIndex].x;
		meshVertexData[dataIndex++] = vertexAttributes.positions[positionIndex].y;
		meshVertexData[dataIndex++] = vertexAttributes.positions[positionIndex].z;

		if (hasNormals)
		{
			meshVertexData[dataIndex++] = vertexAttributes.normals[normalIndex].x;
			meshVertexData[dataIndex++] = vertexAttributes.normals[normalIndex].y;
			meshVertexData[dataIndex++] = vertexAttributes.normals[normalIndex].z;
		}
		if (hasUvs)
		{
			meshVertexData[dataIndex++] = vertexAttributes.uvs[uvIndex].x;
			meshVertexData[dataIndex++] = vertexAttributes.uvs[uvIndex].y;
		}

		EFW_ASSERT((dataIndex * sizeof(float)) <= (size_t)(vertexListCount * outMesh->vertexStride));
	}

	return efwErrs::kOk;
}


void DebugPrintVerticesInfo(const WavefrontObjVertexAttributes& vertexAttributes)
{
	static int32_t previousPositions = 0;
	static int32_t previousNormals = 0;
	static int32_t previousUvs = 0;

	// Data defined since the last group command
	Console::WriteLine("Line %06d: Group New Positions/Normals/Uvs: %d/%d/%d", gModelInputLineCount, 
		vertexAttributes.positions.size() - previousPositions,
		vertexAttributes.normals.size() - previousNormals, 
		vertexAttributes.uvs.size() - previousUvs);

	previousPositions = vertexAttributes.positions.size();
	previousNormals = vertexAttributes.normals.size();
	previousUvs = vertexAttributes.uvs.size();
}


void DebugPrintFacesInfo(const vector<uint64_t>& currentVertexIndexToAttributes)
{
	// TODO
	//int32_t previousMeshIndex = meshes.size() - 1;
}


void DebugPrintLineTokens(const TokenArray* tokenArray)
{
	Console::Write("Line %06d: Tokens [", gModelInputLineCount);
	for (int32_t i=0; i<tokenArray->count; i++)
	{
		if (i < (tokenArray->count-1))
			Console::Write("%s ", StringHelper::GetTokenAt(tokenArray, i));
		else
			Console::Write("%s", StringHelper::GetTokenAt(tokenArray, i));
	}
	Console::WriteLine("]");
}


void DebugPrintMeshInfo(const UnprocessedTriMesh& mesh)
{
	Console::WriteLine("Line %06d: Mesh Name/Vertices/Indices: %s/%d/%d", gModelInputLineCount, mesh.name, mesh.vertexCount, mesh.indexCount);
}


int32_t WavefrontObjReader::ReadMaterialLib(UnprocessedMaterialLib** outMaterial, const char* fullFilePath, ReadFileFunc_t readFileFunc)
{
	if (outMaterial == NULL || fullFilePath == NULL)
		return efwErrs::kInvalidInput;

	if (readFileFunc == NULL)
		readFileFunc = FileReader::ReadAll;

	const int32_t kRequiredAlignment = 1024;
	void* materialFileData = NULL;
	int32_t materialFileDataSize = 0;
	(*readFileFunc)(&materialFileData, &materialFileDataSize, fullFilePath, kRequiredAlignment);
	
	vector<UnprocessedMaterial> materials;
	materials.reserve(1024);
	
	TokenArray* tokenArray;
	StringHelper::CreateTokenArray(&tokenArray, sizeof(Token512), 2);
	const char* kMaterialDelimiters = " \t";

	gMaterialInputLineCount = 0;
	int32_t fileDataIndex = 0;
	while (fileDataIndex < materialFileDataSize)
	{
		int32_t readedBytes = StringHelper::GetLineTokens(tokenArray, &((char*)materialFileData)[fileDataIndex], kMaterialDelimiters);
		fileDataIndex += readedBytes;
		gMaterialInputLineCount++;

		// Skip blank lines
		if (tokenArray->count == 0)
			continue;
		else if (tokenArray->count < 2)
		{
			//Console::WriteLine("Line %06d: [SKIPPED] Invalid or not supported data!", gMaterialInputLineCount);
			continue;
		}

		const char* tokenKey = StringHelper::GetTokenAt(tokenArray, 0);
		const char* tokenValue = StringHelper::GetTokenAt(tokenArray, 1);

		if (strcmp(tokenKey, "newmtl") == 0)
		{
			int32_t materialIndex = materials.size();
			materials.resize(materials.size() + 1);
			materials[materialIndex].id = efwHash64(tokenValue, strlen(tokenValue));
		}
		// Textures
		else 
		{
			bool isAlbedoTexture = (strcmp(tokenKey, "map_Kd") == 0);
			bool isNormalTexture = (strcmp(tokenKey, "map_bump") == 0);
		
			if (isAlbedoTexture || isNormalTexture)
			{
				int32_t materialIndex = materials.size() - 1;
				char directoryPath[Path::kMaxDirectoryLength];
				char fullTexturePath[Path::kMaxFullPathLength];

				PathHelper::GetDirectory(directoryPath, Path::kMaxDirectoryLength, fullFilePath);
				PathHelper::Combine(fullTexturePath, Path::kMaxFullPathLength, directoryPath, StringHelper::GetTokenAt(tokenArray, 1));
			
				if (isAlbedoTexture)
					TextureReader::ReadImage(&materials[materialIndex].albedoTexture, fullTexturePath);
				else if (isNormalTexture)
					TextureReader::ReadImage(&materials[materialIndex].normalMapTexture, fullTexturePath);
			}
		}
	}
	// Release data
	StringHelper::DestroyTokenArray(&tokenArray);
	SAFE_ALIGNED_FREE(materialFileData);

	UnprocessedMaterialLib* materialLib = NULL;
	int32_t materialCount = materials.size();
	if (materialCount > 0)
	{
		materialLib = (UnprocessedMaterialLib*)memalign(1024, sizeof(UnprocessedMaterialLib) + materialCount * sizeof(UnprocessedMaterial));
		materialLib->materialCount = materialCount;
		memcpy(materialLib->materials, &materials[0], materialCount * sizeof(UnprocessedMaterial));
	}

	*outMaterial = materialLib;

	return efwErrs::kOk;
}


int32_t WavefrontObjReader::ReadModelAndMaterials(UnprocessedTriModel** outModel, UnprocessedMaterialLib** outMaterialLib, const char* fullFilePath,
	ReadFileFunc_t readFileFunc)
{
	if (fullFilePath == NULL || outModel == NULL || outMaterialLib == NULL)
		return efwErrs::kInvalidInput;

	if (readFileFunc == NULL)
		readFileFunc = FileReader::ReadAll;

	// Get file directory
	char currentDirectoryPath[Path::kMaxDirectoryLength];
	PathHelper::GetDirectory(currentDirectoryPath, Path::kMaxDirectoryLength, fullFilePath);

	// Read OBJ file data
	const int32_t kRequiredAlignment = 1024;
	void* objFileData = NULL;
	int32_t objFileDataSize = 0;
	(*readFileFunc)(&objFileData, &objFileDataSize, fullFilePath, kRequiredAlignment);

	TokenArray* tokenArray = NULL;
	StringHelper::CreateTokenArray(&tokenArray, sizeof(Token32), 8);

	// Vertex attributes are (NEVER erased and always appended)
	WavefrontObjVertexAttributes vertexAttributes;
	// Index data for the current mesh
	vector<int32_t> currentIndexData;
	// List of all vertices for the current mesh (erased for each new group/mesh found)
	// Each element maps the vertex index to its attributes
	vector<uint64_t> currentVertexIndexToAttributes;
	// Each element maps attributes of one vertex to its index
	map<uint64_t, int32_t> currentVertexAttributesToIndex;
	vector<UnprocessedTriMesh> meshes;
	UnprocessedMaterialLib* materialLib = NULL;

	// Reserve some initial memory
	const uint32_t kVertexAttributesReserveSize = 64 * 1024;
	vertexAttributes.positions.reserve(kVertexAttributesReserveSize);
	vertexAttributes.normals.reserve(kVertexAttributesReserveSize);
	vertexAttributes.uvs.reserve(kVertexAttributesReserveSize);
	currentVertexIndexToAttributes.reserve(kVertexAttributesReserveSize);
	const uint32_t kMeshesReserveSize = 64;
	meshes.reserve(kMeshesReserveSize);
	meshes.resize(meshes.size() + 1);

	// Reset global line counter
	gModelInputLineCount = 0;

	int32_t fileDataIndex = 0;
	while (fileDataIndex < objFileDataSize)
	{
		const char* kObjFormatDemiliters = " \t";
		int32_t readedBytes = StringHelper::GetLineTokens(tokenArray, &((char*)objFileData)[fileDataIndex], kObjFormatDemiliters);
		fileDataIndex += readedBytes;
		gModelInputLineCount++;

		// Debug tokens
		//DebugPrintLineTokens(tokenArray);

		// Skip blank lines
		if (tokenArray->count == 0)
			continue;

		const char* tokenKey = StringHelper::GetTokenAt(tokenArray, 0);
		const char* tokenValue = StringHelper::GetTokenAt(tokenArray, 1);

		char firstSymbol = tokenKey[0];
		switch (firstSymbol)
		{
			// Vertex attribute
		case 'v':
			ParseVertexAttribute(&vertexAttributes, tokenArray);
			break;

			// Face (index vertices attributes per face)
		case 'f':
			ParseFace(&currentIndexData, &currentVertexIndexToAttributes, &currentVertexAttributesToIndex, vertexAttributes, tokenArray);
			break;

			// Group (everything from this point until the next group or EOF belongs to this group
		case 'g':
			{
				int32_t meshIndex = meshes.size() - 1;

				// Checks whether there are index information from a previous mesh before starting a new group.
				if (currentIndexData.size() > 0)
				{
					DebugPrintFacesInfo(currentVertexIndexToAttributes);

					GenerateMesh(&meshes[meshIndex], vertexAttributes, currentVertexIndexToAttributes, currentIndexData);
					DebugPrintMeshInfo(meshes[meshIndex]);

					meshes.resize(meshes.size() + 1);
					meshIndex++;
				}

				// Clear previous data
				currentVertexIndexToAttributes.clear();
				currentVertexAttributesToIndex.clear();
				currentIndexData.clear();

				// Read new mesh name
				if (tokenArray->count > 1)
				{
					strcpy(meshes[meshIndex].name, StringHelper::GetTokenAt(tokenArray, 1));
				}

				// Print vertices up to this point
				DebugPrintVerticesInfo(vertexAttributes);
			}
			break;

			// Material Library (mtlib)
		case 'm':
			{
				if (tokenArray->count > 1 && (strcmp(tokenKey, "mtllib") == 0))
				{
					const char* materialFileName = StringHelper::GetTokenAt(tokenArray, 1);
					char materialFullFilePath[Path::kMaxFullPathLength];
					PathHelper::Combine(materialFullFilePath, Path::kMaxFullPathLength, currentDirectoryPath, materialFileName);

					ReadMaterialLib(&materialLib, materialFullFilePath, readFileFunc);
				}
				else
				{
					//Console::WriteLine("Line %06d: [SKIPPED] \"%s\" is missing a second argument!", gModelInputLineCount, 
					//	StringHelper::GetTokenAt(tokenArray, 0) );
				}
			}
			break;

			// Material (usemtl)
		case 'u':
			{
				if (tokenArray->count > 1 && (strcmp(tokenKey, "usemtl") == 0) &&
					materialLib != NULL && meshes.size() > 0)
				{
					int64_t hash = efwHash64(tokenValue, strlen(tokenValue));
					
					int32_t i=0;
					bool materialFound = false;
					while (i<materialLib->materialCount && !materialFound)
					{
						if (hash == materialLib->materials[i].id)
						{
							int32_t previousMeshIndex = meshes.size() - 1;
							meshes[previousMeshIndex].materialId = i;
							materialFound = true;
						}

						i++;
					}

					if (!materialFound)
					{
						//Console::WriteLine("Line %06d: [SKIPPED] \"%s\" material was not found!", gModelInputLineCount, tokenValue);
					}
				}
			}
			break;

			// Comments - IGNORED
		case '#':
			break;

			// Unhandled symbold
		default:
			{
				//Console::WriteLine("Line %06d: [SKIPPED] \"%s\" is not supported!", gModelInputLineCount, 
				//	StringHelper::GetTokenAt(tokenArray, 0) );
			}
			break;
		}
	}

	DebugPrintVerticesInfo(vertexAttributes);
	DebugPrintFacesInfo(currentVertexIndexToAttributes);

	// 
	if (currentIndexData.size() > 0)
	{
		if (meshes.size() == 0)
		{
			// A group was never started
		}

		int32_t previousMeshIndex = meshes.size() - 1;
		GenerateMesh(&meshes[previousMeshIndex], vertexAttributes, currentVertexIndexToAttributes, currentIndexData);

		DebugPrintMeshInfo(meshes[previousMeshIndex]);
	}

	// Check if last mesh is empty
	else if (meshes.size() > 0)
	{
		int32_t meshIndex = meshes.size() - 1;
		if (meshes[meshIndex].indexCount == 0)
		{
			meshes.pop_back();
		}
	}

	StringHelper::DestroyTokenArray(&tokenArray);
	SAFE_ALIGNED_FREE(objFileData);

	// Flatten all meshes
	UnprocessedTriModel* model = NULL;
	const int32_t meshCount = meshes.size();
	if (meshCount > 0)
	{
		int32_t modelMemory = meshes.size() * sizeof(UnprocessedTriMesh) + sizeof(UnprocessedTriModel);

		model = (UnprocessedTriModel*)memalign(128, modelMemory);
		model->meshCount = meshCount;
		memcpy(&model->meshes[0], &meshes[0], meshCount * sizeof(UnprocessedTriMesh));
	}

	// Copy out
	*outModel = model;
	*outMaterialLib = materialLib;

	return efwErrs::kOk;
}

/*
int32_t WavefrontObjReader::ReadModelFromStream(UnprocessedTriModel** outModel, const void* objFileData, uint32_t objFileDataSize)
{
	if (outModel == NULL || objFileData == NULL || objFileDataSize == 0)
		return efwErrs::kInvalidInput;

	TokenArray* tokenArray;
	StringHelper::CreateTokenArray(&tokenArray, sizeof(Token32), 8);

	// Vertex attributes are (NEVER erased and always appended)
	WavefrontObjVertexAttributes vertexAttributes;
	// Index data for the current mesh
	vector<int32_t> currentIndexData;
	// List of all vertices for the current mesh (erased for each new group/mesh found)
	// Each element maps the vertex index to its attributes
	vector<uint64_t> currentVertexIndexToAttributes;
	// Each element maps attributes of one vertex to its index
	map<uint64_t, int32_t> currentVertexAttributesToIndex;
	vector<UnprocessedTriMesh> meshes;

	// Reserve some initial memory
	const uint32_t kVertexAttributesReserveSize = 64 * 1024;
	vertexAttributes.positions.reserve(kVertexAttributesReserveSize);
	vertexAttributes.normals.reserve(kVertexAttributesReserveSize);
	vertexAttributes.uvs.reserve(kVertexAttributesReserveSize);
	currentVertexIndexToAttributes.reserve(kVertexAttributesReserveSize);
	const uint32_t kMeshesReserveSize = 64;
	meshes.reserve(kMeshesReserveSize);
	meshes.resize(meshes.size() + 1);

	// Reset global line counter
	gModelInputLineCount = 0;

	uint32_t fileDataIndex = 0;
	while (fileDataIndex < objFileDataSize)
	{
		const char* kObjFormatDemiliters = " \t";
		int32_t readedBytes = StringHelper::GetLineTokens(tokenArray, &((char*)objFileData)[fileDataIndex], kObjFormatDemiliters);
		fileDataIndex += readedBytes;
		gModelInputLineCount++;

		// Debug tokens
		//DebugPrintLineTokens(tokenArray);

		// Skip blank lines
		if (tokenArray->count == 0)
			continue;

		char firstSymbol = tokenArray[0].data[0];
		switch (firstSymbol)
		{
			// Vertex attribute
		case 'v':
			ParseVertexAttribute(&vertexAttributes, tokenArray);
			break;

			// Face (index vertices attributes per face)
		case 'f':
			ParseFace(&currentIndexData, &currentVertexIndexToAttributes, &currentVertexAttributesToIndex, vertexAttributes, tokenArray);
			break;

			// Group (everything from this point until the next group or EOF belongs to this group
		case 'g':
			{
				// Checks whether there are index information from a previous mesh before starting a new group.
				if (currentIndexData.size() > 0)
				{
					DebugPrintFacesInfo(currentVertexIndexToAttributes);

					int32_t previousMeshIndex = meshes.size() - 1;
					GenerateMesh(&meshes[previousMeshIndex], vertexAttributes, currentVertexIndexToAttributes, currentIndexData);
					meshes.resize(meshes.size() + 1);

					DebugPrintMeshInfo(meshes[previousMeshIndex]);
				}

				// Clear previous data
				currentVertexIndexToAttributes.clear();
				currentVertexAttributesToIndex.clear();
				currentIndexData.clear();

				// Read new mesh name
				if (tokenArray->count > 1)
				{
					int32_t meshIndex = meshes.size() - 1;
					strcpy(meshes[meshIndex].name, StringHelper::GetTokenAt(tokenArray, 1));
				}

				// Print vertices up to this point
				DebugPrintVerticesInfo(vertexAttributes);
			}
			break;

			// Material Library (mtlib)
		case 'm':
			break;

			// Material (usemtl)
		case 'u':
			// Hash material name using 64b int?
			break;

			// Comments - IGNORED
		case '#':
			break;

		default:
			//Console::WriteLine("Line %06d: [SKIPPED] \"%s\" is not supported!", gModelInputLineCount, 
			//	StringHelper::GetTokenAt(tokenArray, 0) );
			break;
		}
	}

	DebugPrintVerticesInfo(vertexAttributes);
	DebugPrintFacesInfo(currentVertexIndexToAttributes);

	// 
	if (currentIndexData.size() > 0)
	{
		int32_t previousMeshIndex = meshes.size() - 1;
		GenerateMesh(&meshes[previousMeshIndex], vertexAttributes, currentVertexIndexToAttributes, currentIndexData);

		DebugPrintMeshInfo(meshes[previousMeshIndex]);
	}

	// Flatten all meshes
	const int32_t meshCount = meshes.size();
	int32_t modelMemory = meshes.size() * sizeof(UnprocessedTriMesh) + sizeof(UnprocessedTriModel);

	UnprocessedTriModel* model = (UnprocessedTriModel*)memalign(128, modelMemory);
	model->meshCount = meshCount;
	memcpy(&model->meshes[0], &meshes[0], meshCount * sizeof(UnprocessedTriMesh));

	// Copy out
	StringHelper::DestroyTokenArray(&tokenArray);
	*outModel = model;

	return efwErrs::kOk;
}
*/