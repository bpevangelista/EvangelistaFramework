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

#if defined _MSC_VER
#define _CRT_SECURE_NO_WARNINGS		// Disable non-secure function calls warning
#pragma warning(disable:4200)		// Disable zero sized arrays warning
#pragma warning(disable:4127)		// Disable "conditional expression is constant"
#endif

//
#include <stdint.h>
#include <float.h>
#include <limits.h>

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

// Platform specific includes
#if defined _MSC_VER
#elif defined __GNUC__
#include <unistd.h>
#endif

// 
#if defined _MSC_VER
#define EFW_INLINE inline
#define EFW_PACKED_BEGIN __pragma(pack(push,1))
#define EFW_PACKED_END __pragma(pack(pop))
#define EFW_ALIGNED_TYPE(_ALIGN_BYTES, _TYPE) __declspec(align(_ALIGN_BYTES)) _TYPE

EFW_INLINE void* memalign(size_t alignment, size_t size) { return _aligned_malloc(size, alignment); }
EFW_INLINE void freealign(void* address) { _aligned_free(address); }

// GNUC
#elif defined __GNUC__
#define EFW_INLINE inline
#define EFW_PACKED_BEGIN
#define EFW_PACKED_END __attribute__((packed))
#define EFW_ALIGNED_TYPE(_ALIGN_BYTES, _TYPE) _TYPE __attribute__((aligned(_ALIGN_BYTES)))

EFW_INLINE void freealign(void* address) { free(address); }

#else
#error Compiled not defined!
#endif

#ifndef NULL
#define NULL 0L
#endif

// Only works with power of 2
#ifndef EFW_ALIGN
#define EFW_ALIGN(alignment, value) (((value)+(alignment)-1) & ~((alignment)-1))
#endif

#ifndef EFW_SAFE_ALIGNED_FREE
#define EFW_SAFE_ALIGNED_FREE(a) do { if((a) != NULL) { freealign(a); (a) = NULL; } } while(false)
#endif

// TODO: Needs to be improved
#ifndef EFW_COUNTOF
#define EFW_COUNTOF(a) (sizeof(a)/sizeof(*(a)))
#endif

#ifndef EFW_UNUSED
#define EFW_UNUSED(a) (void)(a)
#endif

#ifndef EFW_DEBUG_BREAK
#define EFW_DEBUG_BREAK __debugbreak()
#endif

#ifndef EFW_NOT_IMPLEMENTED_YET
#define EFW_NOT_IMPLEMENTED_YET do { /*TODO*/ } while(false)
#endif

// Temporary: TODO FIX
#include <assert.h>
#ifndef EFW_ASSERT
	#if defined _DEBUG
	#define EFW_ASSERT(a) assert(a)
	#else 
	#define EFW_ASSERT(a) EFW_UNUSED(a)
	#endif
#endif

// 
namespace Path
{
	const int32_t kMaxDirectoryLength = 1024;
	const int32_t kMaxFileNameLength = 256;
	const int32_t kMaxFullPathLength = EFW_ALIGN(4, (kMaxDirectoryLength + kMaxFileNameLength));
};

// Generic types
namespace efwErrs
{
	const int32_t kOk = 0;
	const int32_t kCorruptedData = 1;
	const int32_t kInvalidInput = 2;
	const int32_t kInvalidState = 3;
	const int32_t kOperationFailed = 4;
}

namespace efw
{
	EFW_INLINE void SafeFreeAlign(void* data) { if (data != NULL) { freealign(data); } }
	//EFW_INLINE void SafeFree(void* data) { if (data != NULL) { free(data); } }
	//EFW_INLINE void SafeDelete(void* data) { if (data != NULL) { delete data; } }
	//EFW_INLINE void SafeDeleteArray(void* data) { if (data != NULL) { delete[] data; } }

	class NonCopyable
	{
	protected:
		NonCopyable() {}
		~NonCopyable() {}

	private:
		NonCopyable(const NonCopyable& ref) { EFW_UNUSED(ref); }
		NonCopyable& operator = (const NonCopyable& ref) { EFW_UNUSED(ref); return *this; }
	};


	struct Handle
	{
		uint64_t tableIndex;
		uint64_t guid;
	};


	struct HandleSet
	{
		int32_t count;
		Handle handles[];
	};
}