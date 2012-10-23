#include "Foundation/efwFile.h"

using namespace efw;

int32_t File::GetInfo(FileInfo* outFileInfo, const char* filename)
{
	memset(outFileInfo, 0, sizeof(FileInfo));

#ifdef _MSC_VER
	struct __stat64 fileStats;
	int result = _stat64(filename, &fileStats);
#else
	struct stat fileStats;
	int result = stat(filename, &fileStats);
#endif
	
	if (result == 0)
	{
		outFileInfo->exists = true;
		outFileInfo->size = (size_t)fileStats.st_size;
		return efwErrs::kOk;
	}

	return efwErrs::kOperationFailed;
}