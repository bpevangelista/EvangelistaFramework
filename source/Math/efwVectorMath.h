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
#include "Math/efwMath.h"

//#define EFW_MATH_SSE

#if defined(EFW_MATH_SSE)
#include <mmintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <pmmintrin.h>
#include <tmmintrin.h>
#include <smmintrin.h>
#if defined(_WIN32)
#include <intrin.h>
#endif
typedef __m128 EFW_INTERNAL_VECTOR128;
typedef EFW_INTERNAL_VECTOR128 Vec3fRef;
#else
typedef float EFW_INTERNAL_VECTOR128[4];
namespace efw{ namespace Math { struct Vec3f; } }
typedef const efw::Math::Vec3f& Vec3fRef;
#endif

namespace efw
{
namespace Math
{

	EFW_ALIGNED_TYPE(16, struct) Vec3f
	{
		EFW_INTERNAL_VECTOR128 v;

		Vec3f();
		Vec3f(float X, float Y, float Z);
		Vec3f(const Vec3f&);
		explicit Vec3f(float);
		explicit Vec3f(const float*);

		Vec3f& operator += (Vec3fRef);
		Vec3f& operator -= (Vec3fRef);
		Vec3f& operator *= (Vec3fRef);
		Vec3f& operator /= (Vec3fRef);
		Vec3f& operator *= (float);
		Vec3f& operator /= (float);

		Vec3f operator + () const;
		Vec3f operator - () const;

		Vec3f operator + (Vec3fRef) const;
		Vec3f operator - (Vec3fRef) const;
		Vec3f operator * (Vec3fRef) const;
		Vec3f operator / (Vec3fRef) const;
		Vec3f operator * (float) const;
		Vec3f operator / (float) const;

		friend Vec3f operator * (float, Vec3fRef);
		friend Vec3f operator / (float, Vec3fRef);

		bool operator == (Vec3fRef v) const;
		bool operator != (Vec3fRef v) const;

		// Slow single component access
		float X() const;
		float Y() const;
		float Z() const;

		// Static
		static const Vec3f kZero;
	};

	// 
	bool Vec3IsValid(Vec3fRef v);
	Vec3f Vec3Abs(Vec3fRef v);
	Vec3f Vec3Length(Vec3fRef v);
	Vec3f Vec3LengthSquared(Vec3fRef v);
	Vec3f Vec3Normalize(Vec3fRef v);
	Vec3f Vec3Dot(Vec3fRef vec1, Vec3fRef vec2);
	Vec3f Vec3Cross(Vec3fRef vec1, Vec3fRef vec2);
	Vec3f Vec3Min(Vec3fRef vec1, Vec3fRef vec2);
	Vec3f Vec3Max(Vec3fRef vec1, Vec3fRef vec2);
	Vec3f Vec3Lerp(Vec3fRef vec1, Vec3fRef vec2, float factor);

	// Inline file
	#include "efwVectorMath-inl.h"
}
}