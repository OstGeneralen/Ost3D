// Ost3D Common - Vector4.cpp

#include "Vector4.h"
#include "Vector3.h"
#include "Mat4x4.h"
#include "../Utility/Utilities.h"

// ------------------------------------------------------------
// Ctor
// ------------------------------------------------------------

Math::Vector4f::Vector4f()
	: Vector4f(0.0f)
{
}

Math::Vector4f::Vector4f(float xyzw)
	: Vector4f(xyzw, xyzw, xyzw, xyzw)
{
}

Math::Vector4f::Vector4f(const Vector3f& v)
	: Vector4f(v, 0.0f)
{
}

Math::Vector4f::Vector4f(const Vector3f& v, float w)
	: Vector4f(v.X, v.Y, v.Z, w)
{
}

Math::Vector4f::Vector4f(const Vector4f& v)
	: Vector4f(v.X, v.Y, v.Z, v.W)
{
}

Math::Vector4f::Vector4f(float x, float y, float z, float w)
	: X{x}
	, Y{y}
	, Z{z}
	, W{w}
{
}

Math::Vector4f::Vector4f(Vector4f&& v) noexcept
	: X{v.X}
	, Y{v.Y}
	, Z{v.Z}
	, W{v.W}
{
}

// ------------------------------------------------------------
// Ops
// ------------------------------------------------------------

Math::Vector4f& Math::Vector4f::operator=(const Vector4f& v)
{
	Util::MemCopy(this, &v, sizeof(Vector4f));
	return *this;
}

Math::Vector4f Math::Vector4f::operator*(const Mat4x4& m) const
{
	return Vector4f(Dot(m.Col1()), Dot(m.Col2()), Dot(m.Col3()), Dot(m.Col4()));
}

bool Math::Vector4f::operator==(const Vector4f& v) const
{
	return (FltEquals(X, v.X)) && (FltEquals(Y, v.Y)) && (FltEquals(Z, v.Z)) && (FltEquals(W, v.W));
}

float Math::Vector4f::Dot(const Vector4f& v) const
{
	return (X * v.X) + (Y * v.Y) + (Z * v.Z) + (W * v.W);
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------