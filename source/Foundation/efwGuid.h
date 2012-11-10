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
#include "Foundation/efwPlatform.h"
#include "Foundation/efwMemory.h"

namespace efw
{
	
	struct Guid
	{
		static const uint32_t kMaxNameLength = 64;
		uint64_t hash64;

		void initFromRandomSeed() { 
			uint64_t value = ((uint64_t)(rand() & 0xf) << 59) | ((uint64_t)(rand() & 0x7fff) << 44) | ((uint64_t)(rand() & 0x7fff) << 29) | ((uint64_t)(rand() & 0x7fff) << 14) | ((uint64_t)rand() & 0x0000000000007fff);
			initFromRandomSeed(value);
		}

		void initFromRandomSeed(uint64_t seed) { 
			hash64 = seed;
			SetName("{%llu}", hash64);
		}

		void initFromName(const char* uniqueName) { 
			hash64 = efwHash64(uniqueName);
			SetName(uniqueName);
		}

		bool operator == (const Guid& guid) const {
			return hash64 == guid.hash64;
		}

		bool operator != (const Guid& guid) const {
			return hash64 != guid.hash64;
		}

#ifdef _DEBUG
		const char* GetName() const { return fullName; }
		
	private: 
		char fullName[kMaxNameLength];
		void SetName(const char* str, ...) { 
			va_list args;
			va_start(args, str);
			vsprintf_s(fullName, kMaxNameLength-1, str, args);
			va_end(args);
			
			fullName[kMaxNameLength-1] = 0;
		}
#else
		const char* GetName() const { return ""; }
		void SetName(const char* str, ...) { EFW_UNUSED(str); }
#endif
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