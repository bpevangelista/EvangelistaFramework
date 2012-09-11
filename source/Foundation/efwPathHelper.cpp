#include "Foundation/efwPlatform.h"
#include "Foundation/efwPathHelper.h"
#include "Foundation/efwStringHelper.h"

using namespace efw;

const char kPlatformDirectorySeparator1 = '\\';
const char kPlatformDirectorySeparator2 = '/';


int32_t PathHelper::GetDirectory(char* outPath, int32_t outPathLength, const char* fullPath)
{
	if (outPath == NULL || outPathLength <= 0 || fullPath == NULL)
		return efwErrs::kInvalidInput;

	int32_t directoryLength = 0;

	int32_t result1 = StringHelper::LastIndexOf(fullPath, kPlatformDirectorySeparator1);
	int32_t result2 = StringHelper::LastIndexOf(fullPath, kPlatformDirectorySeparator2);
	int32_t result = MAX(result1, result2);
	if (result != -1)
		directoryLength = result;

	if (directoryLength > 0)
	{
		directoryLength = MIN(directoryLength, outPathLength-1);
		memcpy(outPath, fullPath, directoryLength);
	}
	outPath[directoryLength] = '\0';

	return efwErrs::kOk;
}

//int32_t GetFileName(char* outPath, int32_t outPathLength, const char* fullPath);
//int32_t GetFileExtension(char* outPath, int32_t outPathLength, const char* path);
//int32_t GetFileNameWithoutExtension(char* outPath, int32_t outPathLength, const char* fullPath);

int32_t PathHelper::Combine(char* outPath, int32_t outPathLength, const char* path, const char* fileName)
{
	if (outPath == NULL || outPathLength <= 0 || path == NULL || fileName == NULL)
		return efwErrs::kInvalidInput;

	int32_t pathSize = strlen(path);
	int32_t fileNameSize = strlen(fileName);
	if (outPathLength < (pathSize+fileNameSize+2))	// Add '\\' directory separator and '\0'
		return efwErrs::kInvalidOperation;

	int32_t index = 0;
	memcpy(&outPath[index], path, pathSize);
	index += pathSize;
	outPath[index++] = kPlatformDirectorySeparator1;

	memcpy(&outPath[index], fileName, fileNameSize);
	index += fileNameSize;
	outPath[index++] = '\0';

	return efwErrs::kOk;
}
