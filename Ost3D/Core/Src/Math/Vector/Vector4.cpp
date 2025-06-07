// Ost3D::Core - Kasper "OstGeneralen" Esbjornsson - 2025

#include "Public/Math/Vector/Vector4.h"

#include "Public/Math/Vector/Vector3.h"
#include "Public/Math/Matrix/Matrix4x4.h"
#include "Public/Math/MathFunctions.h"

#include "Public/Utility/Memory/MemoryOps.h"

// ------------------------------------------------------------
// Ctor
// ------------------------------------------------------------

ost::math::Vector4f::Vector4f()
	: Vector4f(0.0f)
{
}

ost::math::Vector4f::Vector4f(float xyzw)
	: Vector4f(xyzw, xyzw, xyzw, xyzw)
{
}

ost::math::Vector4f::Vector4f(const Vector3f& v)
	: Vector4f(v, 0.0f)
{
}

ost::math::Vector4f::Vector4f(const Vector3f& v, float w)
	: Vector4f(v.X, v.Y, v.Z, w)
{
}

ost::math::Vector4f::Vector4f(const Vector4f& v)
	: Vector4f(v.X, v.Y, v.Z, v.W)
{
}

ost::math::Vector4f::Vector4f(float x, float y, float z, float w)
	: X{ x }
	, Y{ y }
	, Z{ z }
	, W{ w }
{
}

ost::math::Vector4f::Vector4f(Vector4f&& v) noexcept
	: X{ v.X }
	, Y{ v.Y }
	, Z{ v.Z }
	, W{ v.W }
{
}

// ------------------------------------------------------------
// Ops
// ------------------------------------------------------------

ost::math::Vector4f& ost::math::Vector4f::operator=(const Vector4f& v)
{
	memory::MemCopy(this, &v, sizeof(Vector4f));
	return *this;
}
ost::math::Vector4f ost::math::Vector4f::operator*(const Mat4x4& m) const
{
	return Vector4f(Dot(m.Col1()), Dot(m.Col2()), Dot(m.Col3()), Dot(m.Col4()));
}

bool ost::math::Vector4f::operator==(const Vector4f& v) const
{
	return (math::FltEquals(X, v.X)) && (math::FltEquals(Y, v.Y)) && (math::FltEquals(Z, v.Z)) && (math::FltEquals(W, v.W));
}

float ost::math::Vector4f::Dot(const Vector4f& v) const
{
	return (X * v.X) + (Y * v.Y) + (Z * v.Z) + (W * v.W);
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------