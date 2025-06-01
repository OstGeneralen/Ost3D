// Ost3D Common - Vector3.h

#pragma once
#include <Common/Math/CommonMath.h>

// ------------------------------------------------------------

namespace Math
{
	class Mat3x3;
	class Vector4f;

	class Vector3f
	{
	public:
		float X, Y, Z;
	public: // Ctor
		Vector3f();
		Vector3f(float xyz);
		Vector3f(float x, float y, float z);
		Vector3f(const Vector3f& v);
		Vector3f(const Vector4f& v);
		Vector3f(Vector3f&& v) noexcept;
		
		~Vector3f() = default;

	public: // Operators
		Vector3f& operator=(const Vector3f& v);
		Vector3f& operator+=(const Vector3f& v);
		Vector3f& operator-=(const Vector3f& v);
		Vector3f& operator*=(float s);
		Vector3f& operator*=(const Mat3x3& m);
		Vector3f& operator/=(float s);

		Vector3f operator+(const Vector3f& v) const;
		Vector3f operator-(const Vector3f& v) const;
		Vector3f operator*(float s) const;

		Vector3f operator*(const Mat3x3& m) const;

		Vector3f operator/(float s) const;

		bool operator==(const Vector3f& v) const;

	public: // Ops
		bool Equals(const Vector3f& v, float tolerance = Constants::FLT_Small) const;
		float Length() const;
		float LengthSq() const;
		Vector3f Normalized() const;
		Vector3f& Normalize();
		float Dot(const Vector3f& v) const;
		Vector3f Cross(const Vector3f& v) const;
	};

	// ------------------------------------------------------------
	// ------------------------------------------------------------
	// ------------------------------------------------------------
}