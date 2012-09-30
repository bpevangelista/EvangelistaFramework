/**
 * Copyright (C) 2012 Bruno P. Evangelista. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 * 
 * THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include "Foundation/efwPlatform.h"
#include <vector>
#include <string.h>

namespace efw
{
	struct Token16 { char data[16]; };
	struct Token32 { char data[32]; };
	struct Token64 { char data[64]; };
	struct Token128 { char data[128]; };
	struct Token256 { char data[256]; };
	struct Token512 { char data[512]; };
	struct Token1024 { char data[1024]; };
	struct Token2048 { char data[2048]; };

	struct TokenArray
	{
		int32_t maxTokens;
		int32_t maxTokenSize;
		int32_t count;
		char data[];
	};

	namespace StringHelper
	{
		int32_t CreateTokenArray(TokenArray** outTokenArray, int32_t tokenSize, int32_t maxTokens);
		void DestroyTokenArray(TokenArray** outTokenArray);
		inline char* GetTokenAt(TokenArray* tokenArray, int32_t tokenIndex) { return (char*)&tokenArray->data[tokenIndex*tokenArray->maxTokenSize]; }
		inline const char* GetTokenAt(const TokenArray* tokenArray, int32_t tokenIndex) { return (const char*)&tokenArray->data[tokenIndex*tokenArray->maxTokenSize]; }

		int32_t IndexOf(const char* str, char value);
		int32_t LastIndexOf(const char* str, char value);

		int32_t GetFirstToken(void* outToken, int32_t tokenSize, const char* str, const char* delimiters, bool allowEmptyToken = false);
		int32_t GetTokens(TokenArray* outTokenArray, const char* str, const char* delimiters, bool allowEmptyToken = false);
		int32_t GetLineTokens(TokenArray* outTokenArray, const char* str, const char* delimiters, bool allowEmptyToken = false);
	}

}