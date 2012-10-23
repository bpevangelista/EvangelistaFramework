#include "Foundation/efwFileReader.h"
#include "Math/efwMath.h"

using namespace efw;

int32_t FileReader::Read(void* outData, uint64_t outSizeInBytes, const char* filename)
{
	FILE* file = fopen(filename, "rb");
	if (file == NULL)
	{
		return efwErrs::kInvalidInput;
	}

	// Get file size
	fseek(file, 0, SEEK_END);
	size_t fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);
	size_t maxReadSize = (size_t)Math::Min((uint64_t)fileSize, outSizeInBytes);

	size_t readedBytes = 0;
	if (outSizeInBytes > 0)
	{
		readedBytes = fread(outData, 1, maxReadSize, file);

		// Retry read if failed
		int32_t retry = 0;
		int32_t kMaxRetries = 4;
		while(readedBytes < maxReadSize && retry < kMaxRetries)
		{
			uint8_t* outDataU8 = (uint8_t*)outData;
			readedBytes += fread(&outDataU8[readedBytes], 1, maxReadSize-readedBytes, file);
			retry++;
		}

	}

	fclose(file);
	return ((readedBytes == fileSize)? efwErrs::kOk : efwErrs::kCorruptedData);
}


int32_t FileReader::ReadAll(void** outData, uint64_t* outSizeInBytes, const char* filename, int32_t requiredAlignment)
{
	FileInfo fileInfo;
	File::GetInfo(&fileInfo, filename);

	// Get file size
	//fseek(file, 0, SEEK_END);
	//size_t fileSize = ftell(file);
	//fseek(file, 0, SEEK_SET);

	size_t fileSize = fileInfo.size;
	uint8_t* fileData = NULL;

	int32_t result = efwErrs::kInvalidInput;
	if (fileSize > 0)
	{
		fileData = (uint8_t*)memalign(requiredAlignment, fileSize);
		result = Read(fileData, fileSize, filename);
	}

	*outData = fileData;
	*outSizeInBytes = fileSize;
	
	return result;
}