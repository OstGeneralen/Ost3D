// Ost3D Common - Vector4.cpp

#include "Vector4.h"
#include <Engine/OstEngineCore.h>

#include "Vector3.h"
#include "../Matrix/Mat4x4.h"

using namespace ost;

// ------------------------------------------------------------
// Ctor
// ------------------------------------------------------------

Vector4f::Vector4f()
	: Vector4f(0.0f)
{
}

Vector4f::Vector4f(float xyzw)
	: Vector4f(xyzw, xyzw, xyzw, xyzw)
{
}

Vector4f::Vector4f(const Vector3f& v)
	: Vector4f(v, 0.0f)
{
}

Vector4f::Vector4f(const Vector3f& v, float w)
	: Vector4f(v.X, v.Y, v.Z, w)
{
}

Vector4f::Vector4f(const Vector4f& v)
	: Vector4f(v.X, v.Y, v.Z, v.W)
{
}

Vector4f::Vector4f(float x, float y, float z, float w)
	: X{x}
	, Y{y}
	, Z{z}
	, W{w}
{
}

Vector4f::Vector4f(Vector4f&& v) noexcept
	: X{v.X}
	, Y{v.Y}
	, Z{v.Z}
	, W{v.W}
{
}

// ------------------------------------------------------------
// Ops
// ------------------------------------------------------------

Vector4f& Vector4f::operator=(const Vector4f& v)
{
	MemCopy(this, &v, sizeof(Vector4f));
	return *this;
}
Vector4f Vector4f::operator*(const Mat4x4& m) const
{
	return Vector4f(Dot(m.Col1()), Dot(m.Col2()), Dot(m.Col3()), Dot(m.Col4()));
}

bool Vector4f::operator==(const Vector4f& v) const
{
	return (math::FltEquals(X, v.X)) && (math::FltEquals(Y, v.Y)) && (math::FltEquals(Z, v.Z)) && (math::FltEquals(W, v.W));
}

float Vector4f::Dot(const Vector4f& v) const
{
	return (X * v.X) + (Y * v.Y) + (Z * v.Z) + (W * v.W);
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------