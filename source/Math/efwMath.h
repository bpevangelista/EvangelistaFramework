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
#include <math.h>

// TODO Temporary
#include <assert.h>
#ifndef EFW_MATH_ASSERT
	#if defined _DEBUG
	#define EFW_MATH_ASSERT(a) assert(a)
	#else 
	#define EFW_MATH_ASSERT(a) EFW_UNUSED(a)
	#endif
#endif


namespace efw
{
	namespace Math
	{
		static const float kEpsilon = 10e-6f;

		EFW_INLINE uint32_t Min(uint32_t v1, uint32_t v2) { return (v1<v2)? v1 : v2; }
		EFW_INLINE uint32_t Max(uint32_t v1, uint32_t v2) { return (v1<v2)? v2 : v1; }
		EFW_INLINE int32_t Min(int32_t v1, int32_t v2) { return (v1<v2)? v1 : v2; }
		EFW_INLINE int32_t Max(int32_t v1, int32_t v2) { return (v1<v2)? v2 : v1; }
		EFW_INLINE float Min(float v1, float v2) { return (v1<v2)? v1 : v2; }
		EFW_INLINE float Max(float v1, float v2) { return (v1<v2)? v2 : v1; }

		EFW_INLINE int32_t Abs(int32_t v) { return abs(v); }
		EFW_INLINE float Abs(float v) { return fabs(v); }
		EFW_INLINE float Ceil(float v) { return ceilf(v); }
		EFW_INLINE float Clamp(float v, float min, float max) { return Math::Max( Math::Min(v, max), min); }
		EFW_INLINE float Floor(float v) { return floorf(v); }
		EFW_INLINE float Sqrt(float v) { return sqrtf(v); }

		EFW_INLINE bool IsNumber(float b) { return (b == b); }
		EFW_INLINE bool IsFinite(float b) { return (b >= -FLT_MAX && b <= FLT_MAX); }
	}

} // efw