// Ost3D::Core - Kasper "OstGeneralen" Esbjornsson - 2025

#include "Public/Math/Vector/Vector3.h"

#include "Public/Math/Matrix/Matrix3x3.h"
#include "Public/Math/Vector/Vector4.h"
#include "Public/Math/MathFunctions.h"

#include "Public/Utility/Memory/MemoryOps.h"

// ------------------------------------------------------------

ost::math::Vector3f::Vector3f()
	: X{ 0.f }
	, Y{ 0.f }
	, Z{ 0.f }
{
}

ost::math::Vector3f::Vector3f(float xyz)
	: X{ xyz }
	, Y{ xyz }
	, Z{ xyz }
{
}

ost::math::Vector3f::Vector3f(float x, float y, float z)
	: X{ x }
	, Y{ y }
	, Z{ z }
{
}

ost::math::Vector3f::Vector3f(const Vector3f& v)
	: X{ v.X }
	, Y{ v.Y }
	, Z{ v.Z }
{
}

ost::math::Vector3f::Vector3f(const Vector4f& v)
	: X{ v.X }
	, Y{ v.Y }
	, Z{ v.Z }
{
}

ost::math::Vector3f::Vector3f(Vector3f&& v) noexcept
	: X{ v.X }
	, Y{ v.Y }
	, Z{ v.Z }
{
}

// ------------------------------------------------------------
// Operators
// ------------------------------------------------------------

ost::math::Vector3f& ost::math::Vector3f::operator=(const Vector3f& v)
{
	memory::MemCopy(this, &v, sizeof(Vector3f));
	return *this;
}

ost::math::Vector3f& ost::math::Vector3f::operator+=(const Vector3f& v)
{
	X += v.X;
	Y += v.Y;
	Z += v.Z;
	return *this;
}

ost::math::Vector3f& ost::math::Vector3f::operator-=(const Vector3f& v)
{
	X -= v.X;
	Y -= v.Y;
	Z -= v.Z;
	return *this;
}

ost::math::Vector3f& ost::math::Vector3f::operator*=(float s)
{
	X *= s;
	Y *= s;
	Z *= s;
	return *this;
}

ost::math::Vector3f& ost::math::Vector3f::operator*=(const Mat3x3& m)
{
	float x = Dot(m.Col1());
	float y = Dot(m.Col2());
	float z = Dot(m.Col3());
	X = x;
	Y = y;
	Z = z;
	return *this;
}

ost::math::Vector3f& ost::math::Vector3f::operator/=(float s)
{
	X /= s;
	Y /= s;
	Z /= s;
	return *this;
}

ost::math::Vector3f ost::math::Vector3f::operator+(const Vector3f& v) const
{
	return Vector3f(X + v.X, Y + v.Y, Z + v.Z);
}

ost::math::Vector3f ost::math::Vector3f::operator-(const Vector3f& v) const
{
	return Vector3f(X - v.X, Y - v.Y, Z - v.Z);
}

ost::math::Vector3f ost::math::Vector3f::operator*(float s) const
{
	return Vector3f(X * s, Y * s, Z * s);
}

ost::math::Vector3f ost::math::Vector3f::operator*(const Mat3x3& m) const
{
	return Vector3f(Dot(m.Col1()), Dot(m.Col2()), Dot(m.Col3()));
}

ost::math::Vector3f ost::math::Vector3f::operator/(float s) const
{
	return Vector3f(X / s, Y / s, Z / s);
}

bool ost::math::Vector3f::operator==(const Vector3f& v) const
{
	return Equals(v);
}

// ------------------------------------------------------------
// Ops
// ------------------------------------------------------------

bool ost::math::Vector3f::Equals(const Vector3f& v, float tolerance) const
{
	return math::FltEquals(X, v.X, tolerance)
		&& math::FltEquals(Y, v.Y, tolerance)
		&& math::FltEquals(Z, v.Z, tolerance);
}

float ost::math::Vector3f::Length() const
{
	return math::SquareRoot(LengthSq());
}

float ost::math::Vector3f::LengthSq() const
{
	return (X * X) + (Y * Y) + (Z * Z);
}

ost::math::Vector3f ost::math::Vector3f::Normalized() const
{
	return (*this) / Length();
}

ost::math::Vector3f& ost::math::Vector3f::Normalize()
{
	(*this) /= Length();
	return *this;
}

float ost::math::Vector3f::Dot(const Vector3f& v) const
{
	return (X * v.X) + (Y * v.Y) + (Z * v.Z);
}

ost::math::Vector3f ost::math::Vector3f::Cross(const Vector3f& v) const
{
	return Vector3f(
		(Y * v.Z) - (Z * v.Y),
		(Z * v.X) - (X * v.Z),
		(X * v.Y) - (Y * v.X));
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------