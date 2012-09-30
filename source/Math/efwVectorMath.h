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
#define __MMX__
#define __SSE__
#define __SSE2__
#define __SSE3__
#define __SSSE3__
#define __SSE4_2__

#ifdef __MMX__
#include <mmintrin.h>
#endif
#ifdef __SSE__
#include <xmmintrin.h>
#endif
#ifdef __SSE2__
#include <emmintrin.h>
#endif
#ifdef __SSE3__
#include <pmmintrin.h>
#endif
#ifdef __SSSE3__
#include <tmmintrin.h>
#endif
#if defined (__SSE4_2__) || defined (__SSE4_1__)
#include <smmintrin.h>
#endif
#if defined(_WIN32)
#include <intrin.h>
#endif
typedef __m128 EFW_INTERNAL_VECTOR128;
#else
typedef float EFW_INTERNAL_VECTOR128[4];
#endif

namespace efw
{
namespace Math
{

	EFW_ALIGNED_TYPE(16, struct) Vec3f
	{
		EFW_INTERNAL_VECTOR128 v;
#if defined(EFW_MATH_SSE)
		typedef EFW_INTERNAL_VECTOR128 Vec3fRef;
#else
		typedef const Vec3f& Vec3fRef;
#endif

		Vec3f();
		Vec3f(float);
		Vec3f(float X, float Y, float Z);
		Vec3f(const float*);
		Vec3f(const Vec3f&);

		operator float* ();
		operator const float* () const;

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

		bool operator == (Vec3fRef v) const;
		bool operator != (Vec3fRef v) const;

		// Slow single component access
		float X() const;
		float Y() const;
		float Z() const;
	};
	typedef Vec3f::Vec3fRef Vec3fRef;

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

	EFW_INLINE Vec3f::Vec3f()
	{
	}

	EFW_INLINE Vec3f::Vec3f(float X)
	{
		EFW_MATH_ASSERT( IsFinite(X) );
#if defined(EFW_MATH_SSE)
		v = _mm_load_ps1(&X);
#else
		v[0] = v[1] = v[2] = X;
#endif
	}

	EFW_INLINE Vec3f::Vec3f(float X, float Y, float Z)
	{
		EFW_MATH_ASSERT( IsFinite(X) && IsFinite(Y) && IsFinite(Z) );
#if defined(EFW_MATH_SSE)
		EFW_ALIGNED_TYPE(16, float) vec[4] = {X, Y, Z, 0};
		v = _mm_load_ps(vec);
#else
		v[0] = X; v[1] = Y; v[2] = Z;
#endif
	}

	EFW_INLINE Vec3f::Vec3f(const float* ptr)
	{
		EFW_MATH_ASSERT( ((&V[0]) % 16) == 0);
		EFW_MATH_ASSERT( IsFinite(ptr[0]) && IsFinite(ptr[1]) && IsFinite(ptr[2]) );

#if defined(EFW_MATH_SSE)
#else
		v[0] = ptr[0];
		v[1] = ptr[1];
		v[2] = ptr[2];
#endif
	}

	EFW_INLINE Vec3f::Vec3f(const Vec3f& vec)
	{
#if defined(EFW_MATH_SSE)
#else
		v[0] = vec.v[0];
		v[1] = vec.v[1];
		v[2] = vec.v[2];
#endif
	}

	EFW_INLINE Vec3f::operator float* ()
	{
#if defined(EFW_MATH_SSE)
#else
		return &v[0];
#endif
	}

	EFW_INLINE Vec3f::operator const float* () const
	{
#if defined(EFW_MATH_SSE)
#else
		return &v[0];
#endif
	}

	EFW_INLINE Vec3f& Vec3f::operator += (Vec3fRef vec)
	{
#if defined(EFW_MATH_SSE)
#else
		v[0] += vec.v[0];
		v[1] += vec.v[1];
		v[2] += vec.v[2];
#endif
		return *this;
	}

	EFW_INLINE Vec3f& Vec3f::operator -= (Vec3fRef vec)
	{
#if defined(EFW_MATH_SSE)
#else
		v[0] -= vec.v[0];
		v[1] -= vec.v[1];
		v[2] -= vec.v[2];
#endif
		return *this;
	}

	EFW_INLINE Vec3f& Vec3f::operator *= (Vec3fRef vec)
	{
#if defined(EFW_MATH_SSE)
#else
		v[0] *= vec[0];
		v[1] *= vec[1];
		v[2] *= vec[2];
#endif
		return *this;
	}

	EFW_INLINE Vec3f& Vec3f::operator /= (Vec3fRef vec)
	{
		EFW_MATH_ASSERT(vec.v[0] != 0 && vec.v[1] != 0 && vec.v[2] != 0);

#if defined(EFW_MATH_SSE)
#else
		v[0] /= vec.v[0];
		v[1] /= vec.v[1];
		v[2] /= vec.v[2];
#endif
		return *this;
	}

	EFW_INLINE Vec3f& Vec3f::operator *= (float value)
	{
#if defined(EFW_MATH_SSE)
#else
		v[0] *= value;
		v[1] *= value;
		v[2] *= value;
#endif
		return *this;
	}

	EFW_INLINE Vec3f& Vec3f::operator /= (float value)
	{
#if defined(EFW_MATH_SSE)
#else
		float inverseValue = 1.0f / value;
		v[0] *= inverseValue;
		v[1] *= inverseValue;
		v[2] *= inverseValue;
#endif
		return *this;
	}

	EFW_INLINE Vec3f Vec3f::operator + () const
	{
		return *this;
	}

	EFW_INLINE Vec3f Vec3f::operator - () const
	{
#if defined(EFW_MATH_SSE)
#else
		return Vec3f( -v[0], -v[1], -v[2] );
#endif
	}

	EFW_INLINE Vec3f Vec3f::operator + (Vec3fRef vec) const
	{
#if defined(EFW_MATH_SSE)
#else
		return Vec3f(v[0] + vec.v[0], v[1] + vec.v[1], v[2] + vec.v[2]);
#endif
	}

	EFW_INLINE Vec3f Vec3f::operator - (Vec3fRef vec) const
	{
#if defined(EFW_MATH_SSE)
#else
		return Vec3f(v[0] - vec.v[0], v[1] - vec.v[1], v[2] - vec.v[2]);
#endif
	}

	EFW_INLINE Vec3f Vec3f::operator * (Vec3fRef vec) const
	{
#if defined(EFW_MATH_SSE)
#else
		return Vec3f(v[0] * vec.v[0], v[1] * vec.v[1], v[2] * vec.v[2]);
#endif
	}

	EFW_INLINE Vec3f Vec3f::operator / (Vec3fRef vec) const
	{
#if defined(EFW_MATH_SSE)
#else
		return Vec3f(v[0] / vec.v[0], v[1] / vec.v[1], v[2] / vec.v[2]);
#endif
	}

	EFW_INLINE Vec3f Vec3f::operator * (float value) const
	{
#if defined(EFW_MATH_SSE)
#else
		return Vec3f(v[0] * value, v[1] * value, v[2] * value);
#endif
	}

	EFW_INLINE Vec3f Vec3f::operator / (float value) const
	{
		EFW_MATH_ASSERT(value != 0);
#if defined(EFW_MATH_SSE)
#else
		float inverseValue = 1.0f / value;
		return Vec3f(v[0] * inverseValue, v[1] * inverseValue, v[2] * inverseValue);
#endif
	}

	EFW_INLINE Vec3f operator * (float value, Vec3fRef vec)
	{
#if defined(EFW_MATH_SSE)
#else
		return Vec3f(vec.v[0] * value, vec.v[1] * value, vec.v[2] * value);
#endif
	}

	EFW_INLINE bool Vec3f::operator == (Vec3fRef vec) const
	{
#if defined(EFW_MATH_SSE)
#else
		float diffX = Math::Abs(v[0] - vec.v[0]);
		float diffY = Math::Abs(v[1] - vec.v[1]);
		float diffZ = Math::Abs(v[2] - vec.v[2]);
		return (diffX <= Math::kEpsilon && diffY <= Math::kEpsilon && diffZ <= Math::kEpsilon);
#endif
	}

	EFW_INLINE bool Vec3f::operator != (Vec3fRef vec) const
	{
		return !(*this == vec);
	}

	EFW_INLINE float Vec3f::X() const
	{
#if defined(EFW_MATH_SSE)
#else
		return v[0];
#endif
	}

	EFW_INLINE float Vec3f::Y() const
	{
#if defined(EFW_MATH_SSE)
#else
		return v[1];
#endif
	}

	EFW_INLINE float Vec3f::Z() const
	{
#if defined(EFW_MATH_SSE)
#else
		return v[2];
#endif
	}

	bool Vec3IsValid(Vec3fRef v)
	{
#if defined(EFW_MATH_SSE)
		EFW_ALIGNED_TYPE(16, float) values[4];
		_mm_store_ss(&values[0], v);
		return ( IsFinite(values[0]) && IsFinite(values[1]) && IsFinite(values[2]) );
#else
		return ( IsFinite(v[0]) && IsFinite(v[1]) && IsFinite(v[2]) );
#endif
	}

	EFW_INLINE Vec3f Vec3Length(Vec3fRef v)
	{
#if defined(EFW_MATH_SSE)
#else
		return Math::Sqrt( Vec3Dot(v, v)[0] );
#endif
	}

	EFW_INLINE Vec3f Vec3LengthSquared(Vec3fRef v)
	{
#if defined(EFW_MATH_SSE)
#else
		return Vec3Dot(v, v);
#endif
	}
	
	EFW_INLINE Vec3f Vec3Normalize(Vec3fRef v)
	{
#if defined(EFW_MATH_SSE)
#else
		float invLength = 0.0f;
		float lengthSquared = Vec3LengthSquared(v)[0];

		if (lengthSquared > 0.0f)
		{
			float invLength = 1.0f / Math::Sqrt(lengthSquared);
		}
		return Vec3f(v[0] * invLength, v[1] * invLength, v[2] * invLength);
#endif
	}

	EFW_INLINE Vec3f Vec3Dot(Vec3fRef vec1, Vec3fRef vec2)
	{
#if defined(EFW_MATH_SSE)
#else
		float result = vec1.v[0] * vec2.v[0] + vec1.v[1] * vec2.v[1] + vec1.v[2] * vec2.v[2];
		return Vec3f(result, result, result);
#endif
	}

	EFW_INLINE Vec3f Vec3Cross(Vec3fRef vec1, Vec3fRef vec2)
	{
#if defined(EFW_MATH_SSE)
#else
		return (vec1.v[1] * vec2.v[2] - vec1.v[2] * vec2.v[1],
			vec1.v[2] * vec2.v[0] - vec1.v[0] * vec2.v[2],
			vec1.v[0] * vec2.v[1] - vec1.v[1] * vec2.v[0] );
#endif
	}

	EFW_INLINE Vec3f Vec3Abs(Vec3fRef vec)
	{
#if defined(EFW_MATH_SSE)
#else
		return Vec3f( Math::Abs(vec.v[0]), Math::Abs(vec.v[1]), Math::Abs(vec.v[2]) );
#endif
	}

	EFW_INLINE Vec3f Vec3Min(Vec3fRef vec1, Vec3fRef vec2)
	{
#if defined(EFW_MATH_SSE)
#else
		return Vec3f(
			Math::Min(vec1.v[0], vec2.v[0]),
			Math::Min(vec1.v[1], vec2.v[1]),
			Math::Min(vec1.v[2], vec2.v[2]) );
#endif
	}

	EFW_INLINE Vec3f Vec3Max(Vec3fRef vec1, Vec3fRef vec2)
	{
#if defined(EFW_MATH_SSE)
#else
		return Vec3f(
			Math::Max(vec1.v[0], vec2.v[0]),
			Math::Max(vec1.v[1], vec2.v[1]),
			Math::Max(vec1.v[2], vec2.v[2]) );
#endif
	}

	EFW_INLINE Vec3f Vec3Lerp(Vec3fRef vec1, Vec3fRef vec2, float factor)
	{
#if defined(EFW_MATH_SSE)
#else
		return (vec1.v[0] + factor * (vec2.v[0] - vec1.v[0]),
			vec1.v[1] + factor * (vec2.v[1] - vec1.v[1]),
			vec1.v[2] + factor * (vec2.v[2] - vec1.v[2]) );
#endif
	}

}
}