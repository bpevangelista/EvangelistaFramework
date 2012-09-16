#include "Foundation/efwFile.h"

//#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

#if !defined _MSC_VER
#include <unistd.h>
#endif

using namespace efw;

int32_t File::GetInfo(FileInfo* outFileInfo, const char* filename)
{
	memset(outFileInfo, 0, sizeof(FileInfo));

	struct __stat64 stats;
	int result = _stat64(filename, &stats);
	if (result == 0)
	{
		outFileInfo->exists = true;
		outFileInfo->size = (size_t)stats.st_size;
		return efwErrs::kOk;
	}

	return efwErrs::kOperationFailed;
}