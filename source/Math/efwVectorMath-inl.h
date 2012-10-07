//#include "Math/efwVectorMath.h"
//using namespace efw::Math;

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
	EFW_MATH_ASSERT( (((int32_t)ptr) % 16) == 0 );
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
	v[0] *= vec.v[0];
	v[1] *= vec.v[1];
	v[2] *= vec.v[2];
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

EFW_INLINE Vec3f operator * (float value, Vec3fRef vec)
{
#if defined(EFW_MATH_SSE)
#else
	return Vec3f(vec.v[0] * value, vec.v[1] * value, vec.v[2] * value);
#endif
}

EFW_INLINE Vec3f operator / (float value, Vec3fRef vec)
{
	#if defined(EFW_MATH_SSE)
#else
	return Vec3f(value) / vec;
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

EFW_INLINE bool Vec3IsValid(Vec3fRef vec)
{
#if defined(EFW_MATH_SSE)
	EFW_ALIGNED_TYPE(16, float) values[4];
	_mm_store_ss(&values[0], v);
	return ( IsFinite(values[0]) && IsFinite(values[1]) && IsFinite(values[2]) );
#else
	return ( IsFinite(vec.v[0]) && IsFinite(vec.v[1]) && IsFinite(vec.v[2]) );
#endif
}

EFW_INLINE Vec3f Vec3Length(Vec3fRef vec)
{
#if defined(EFW_MATH_SSE)
#else
	float result = Math::Sqrt( Vec3Dot(vec, vec).v[0] );
	return Vec3f(result);
#endif
}

EFW_INLINE Vec3f Vec3LengthSquared(Vec3fRef vec)
{
#if defined(EFW_MATH_SSE)
#else
	return Vec3Dot(vec, vec);
#endif
}
	
EFW_INLINE Vec3f Vec3Normalize(Vec3fRef vec)
{
#if defined(EFW_MATH_SSE)
#else
	float invLength = 0.0f;
	float lengthSquared = Vec3LengthSquared(vec).v[0];

	if (lengthSquared > 0.0f)
	{
		float invLength = 1.0f / Math::Sqrt(lengthSquared);
	}

	return vec * Vec3f(invLength);
#endif
}

EFW_INLINE Vec3f Vec3Dot(Vec3fRef vec1, Vec3fRef vec2)
{
#if defined(EFW_MATH_SSE)
#else
	float result = vec1.v[0] * vec2.v[0] + vec1.v[1] * vec2.v[1] + vec1.v[2] * vec2.v[2];
	return Vec3f(result);
#endif
}

EFW_INLINE Vec3f Vec3Cross(Vec3fRef vec1, Vec3fRef vec2)
{
#if defined(EFW_MATH_SSE)
#else
	return Vec3f(vec1.v[1] * vec2.v[2] - vec1.v[2] * vec2.v[1],
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

EFW_INLINE Vec3f Vec3Ceil(Vec3fRef vec)
{
#if defined(EFW_MATH_SSE)
#else
	return Vec3f(Math::Ceil(vec.v[0]), Math::Ceil(vec.v[1]), Math::Ceil(vec.v[2]));
#endif
}

EFW_INLINE Vec3f Vec3Floor(Vec3fRef vec)
{
#if defined(EFW_MATH_SSE)
#else
	return Vec3f(Math::Floor(vec.v[0]), Math::Floor(vec.v[1]), Math::Floor(vec.v[2]));
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
	return Vec3f(vec1.v[0] + factor * (vec2.v[0] - vec1.v[0]),
		vec1.v[1] + factor * (vec2.v[1] - vec1.v[1]),
		vec1.v[2] + factor * (vec2.v[2] - vec1.v[2]) );
#endif
}

EFW_INLINE void Vec3GetFloats(float* out, Vec3fRef vec)
{
	EFW_MATH_ASSERT(out != NULL);
#if defined(EFW_MATH_SSE)
#else
	out[0] = vec.v[0];
	out[1] = vec.v[1];
	out[2] = vec.v[2];
#endif
}