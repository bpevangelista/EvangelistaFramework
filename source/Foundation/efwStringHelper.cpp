#include "Foundation/efwStringHelper.h"
#include "Foundation/efwMemory.h"
#include "Math/efwMath.h"

#include <string.h>
#include <stdlib.h>

using namespace efw;


int32_t StringHelper::IndexOf(const char* str, char value)
{
	int32_t i=0;
	int32_t contains = -1;

	while (str[i] != '\0' && contains == -1)
	{
		if (str[i] == value)
			contains = i;
		i++;
	}

	return contains;
}


int32_t StringHelper::LastIndexOf(const char* str, char value)
{
	int32_t i=0;
	int32_t contains = -1;

	while (str[i] != '\0')
	{
		if (str[i] == value)
			contains = i;
		i++;
	}

	return contains;
}


int32_t StringHelper::CreateTokenArray(TokenArray** outTokenArray, int32_t tokenSize, int32_t maxTokens)
{
	if (outTokenArray == NULL)
		return efwErrs::kInvalidInput;

	*outTokenArray = (TokenArray*)memalign(16, sizeof(TokenArray) + tokenSize * maxTokens);
	(*outTokenArray)->count = 0;
	(*outTokenArray)->maxTokenSize = tokenSize;
	(*outTokenArray)->maxTokens = maxTokens;

	return 0;
}


void StringHelper::DestroyTokenArray(TokenArray** outTokenArray)
{
	if (outTokenArray != NULL)
	{
		SAFE_ALIGNED_FREE(*outTokenArray);
	}
}


int32_t StringHelper::GetFirstToken(void* outToken, int32_t tokenSize, const char* str, const char* delimiters, bool allowEmptyToken)
{
	if (outToken == NULL || tokenSize <= 0)
		return efwErrs::kInvalidInput;
	if (str == NULL)
	{
		((char*)outToken)[0] = '\0';
		return 0;
	}

	// Initialize to invalid
	int32_t index = 0;
	if (str[index] != '\0')
	{
		// Get rid of delimiters before the token start
		if (!allowEmptyToken)
		{
			while (str[index] != '\0' && StringHelper::IndexOf(delimiters, str[index]) != -1)
				index++;
		}

		int32_t startIndex = index;
		while (str[index] != '\0' && StringHelper::IndexOf(delimiters, str[index]) == -1)
			index++;

		// Generate token
		int32_t copyLength = Math::Min(index - startIndex, tokenSize-1);
		memcpy(outToken, &str[startIndex], copyLength);
		((char*)outToken)[copyLength] = '\0';

		// First character after the first delimiter found
		if (str[index] != '\0')
			index++;
	}

	return index;
}


int32_t StringHelper::GetTokens(TokenArray* outTokenArray, const char* str, const char* delimiters, bool allowEmptyToken)
{
	if (outTokenArray == NULL)
		return efwErrs::kInvalidInput;
	if (str == NULL)
	{
		outTokenArray->count = 0;
		return 0;
	}

	int32_t tokenIndex = 0;
	int32_t strIndex = 0;
	while (str[strIndex] != '\0' && tokenIndex < outTokenArray->maxTokens)
	{
		char* currentToken = StringHelper::GetTokenAt(outTokenArray, tokenIndex);
		strIndex += StringHelper::GetFirstToken(currentToken, outTokenArray->maxTokenSize, str+strIndex, delimiters, allowEmptyToken);
		tokenIndex++;
	}

	outTokenArray->count = tokenIndex;
	return strIndex;
}


/**
 * Tokenize the first line of the input string using the specified delimiters.
 * 
 * @param outTokenArray Output token array, where all generated tokens will be apended.
 * @param str Input string.
 * @param strSize Length of the input string (excluding the '\0' character).
 * @param delimiters Array of delimiters, where each character is a different delimiter.
 * @param delimitersSize Size of the delimiters array.
 * @return Number of characters read and processed including the first delimiter found.
 */
int32_t StringHelper::GetLineTokens(TokenArray* outTokenArray, const char* str, const char* delimiters, bool allowEmptyToken)
{
	if (outTokenArray == NULL || str == NULL)
		return 0;

	const char* kBreakLineDelimiters = "\n";

	// Get one full line as a token
	Token2048 lineToken;
	int32_t index = StringHelper::GetFirstToken(&lineToken, sizeof(lineToken), str, kBreakLineDelimiters, true);

	// Handle \r\n
	//if (lineToken.data[index-1] == '\r')
	//{
	//	lineToken.data[index-1] = '\0';
	//}
	if (lineToken.data[index-2] == '\r')
	{
		lineToken.data[index-2] = '\0';
	}

	// Now get all tokens on that line
	StringHelper::GetTokens(outTokenArray, (char*)&lineToken, delimiters, allowEmptyToken);

	return index;
}
