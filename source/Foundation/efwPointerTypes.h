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

namespace efw
{

template <typename T>
class ScopedPtr : NonCopyable
{
public:
	ScopedPtr(T* ptr = NULL) { mRawPointer = ptr; }
	~ScopedPtr() { EFW_SAFE_ALIGNED_FREE(mRawPointer); }
	
	void Reset(T* ptr = NULL) { if (ptr != mRawPointer) { EFW_SAFE_ALIGNED_FREE(mRawPointer); mRawPointer = ptr; } }
	T* Release() { T* ptr = mRawPointer; mRawPointer = NULL; return ptr; }

	T* operator -> () const { return mRawPointer; }
	T& operator * () const { return *mRawPointer; }
	operator T* () const { return mRawPointer; }
	T& operator [] (int index) const { EFW_ASSERT(index >= 0 && mRawPointer != NULL); return mRawPointer[index]; }

	bool operator == (const T* ptr) const { return mRawPointer == ptr; }
	bool operator != (const T* ptr) const { return mRawPointer != ptr; }

private:
	T* mRawPointer;
};


template <typename T>
class GenericScopedPtr : NonCopyable
{
public:
	typedef void (*DeleteFuncPtr)(void* data);

	GenericScopedPtr(T* ptr = NULL, DeleteFuncPtr funcPtr = efw::SafeFreeAlign) { EFW_ASSERT(funcPtr); mRawPointer = ptr; mFuncPtr = funcPtr; }
	~GenericScopedPtr() { mFuncPtr((void*)mRawPointer); }
	
	void Reset(T* ptr = NULL) { if (ptr != mRawPointer) { EFW_SAFE_ALIGNED_FREE(mRawPointer); mRawPointer = ptr; } }
	T* Release() { T* ptr = mRawPointer; mRawPointer = NULL; return ptr; }

	T* operator -> () const { return mRawPointer; }
	T& operator * () const { return *mRawPointer; }
	operator T* () const { return mRawPointer; }
	T& operator [] (int index) const { EFW_ASSERT(index >= 0 && mRawPointer != NULL); return mRawPointer[index]; }

	bool operator == (const T* ptr) const { return mRawPointer == ptr; }
	bool operator != (const T* ptr) const { return mRawPointer != ptr; }

private:
	T* mRawPointer;
	DeleteFuncPtr mFuncPtr;
};

}