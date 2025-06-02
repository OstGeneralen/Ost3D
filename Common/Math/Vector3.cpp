// Ost3D Common - Vector3.cpp

#include "Mat3x3.h"
#include "Vector3.h"
#include "Vector4.h"
#include "../Utility/Utilities.h"

// ------------------------------------------------------------
// Ctor
// ------------------------------------------------------------

Vector3f::Vector3f()
	: X{0.f}
	, Y{0.f}
	, Z{0.f}
{
}

Vector3f::Vector3f(float xyz)
	: X{xyz}
	, Y{xyz}
	, Z{xyz}
{
}

Vector3f::Vector3f(float x, float y, float z)
	: X{x}
	, Y{y}
	, Z{z}
{
}

Vector3f::Vector3f(const Vector3f& v)
	: X{v.X}
	, Y{v.Y}
	, Z{v.Z}
{
}

Vector3f::Vector3f(const Vector4f& v)
	: X{ v.X }
	, Y{ v.Y }
	, Z{ v.Z }
{
}

Vector3f::Vector3f(Vector3f&& v) noexcept
	: X{v.X}
	, Y{v.Y}
	, Z{v.Z}
{
}

// ------------------------------------------------------------
// Operators
// ------------------------------------------------------------

Vector3f& Vector3f::operator=(const Vector3f& v)
{
	Util::MemCopy(this, &v, sizeof(Vector3f));
	return *this;
}

Vector3f& Vector3f::operator+=(const Vector3f& v)
{
	X += v.X;
	Y += v.Y;
	Z += v.Z;
	return *this;
}

Vector3f& Vector3f::operator-=(const Vector3f& v)
{
	X -= v.X;
	Y -= v.Y;
	Z -= v.Z;
	return *this;
}

Vector3f& Vector3f::operator*=(float s)
{
	X *= s;
	Y *= s;
	Z *= s;
	return *this;
}

Vector3f& Vector3f::operator*=(const Mat3x3& m)
{
	float x = Dot(m.Col1());
	float y = Dot(m.Col2());
	float z = Dot(m.Col3());
	X = x;
	Y = y;
	Z = z;
	return *this;
}

Vector3f& Vector3f::operator/=(float s)
{
	X /= s;
	Y /= s;
	Z /= s;
	return *this;
}

Vector3f Vector3f::operator+(const Vector3f& v) const
{
	return Vector3f(X + v.X, Y + v.Y, Z + v.Z);
}

Vector3f Vector3f::operator-(const Vector3f& v) const
{
	return Vector3f(X - v.X, Y - v.Y, Z - v.Z);
}

Vector3f Vector3f::operator*(float s) const
{
	return Vector3f(X * s, Y * s, Z * s);
}

Vector3f Vector3f::operator*(const Mat3x3& m) const
{
	return Vector3f(Dot(m.Col1()), Dot(m.Col2()), Dot(m.Col3()));
}

Vector3f Vector3f::operator/(float s) const
{
	return Vector3f(X / s, Y / s, Z / s);
}

bool Vector3f::operator==(const Vector3f& v) const
{
	return Equals(v);
}

// ------------------------------------------------------------
// Ops
// ------------------------------------------------------------

bool Vector3f::Equals(const Vector3f& v, float tolerance) const
{
	return Math::FltEquals(X, v.X, tolerance) 
		&& Math::FltEquals(Y, v.Y, tolerance)
		&& Math::FltEquals(Z, v.Z, tolerance);
}

float Vector3f::Length() const
{
	return Math::SquareRoot(LengthSq());
}

float Vector3f::LengthSq() const
{
	return (X * X) + (Y * Y) + (Z * Z);
}

Vector3f Vector3f::Normalized() const
{
	return (*this) / Length();
}

Vector3f& Vector3f::Normalize()
{
	(*this) /= Length();
	return *this;
}

float Vector3f::Dot(const Vector3f& v) const
{
	return (X * v.X) + (Y * v.Y) + (Z * v.Z);
}

Vector3f Vector3f::Cross(const Vector3f& v) const
{
	return Vector3f( 
		(Y * v.Z) - (Z * v.Y),  
		(Z * v.X) - (X * v.Z),
		(X * v.Y) - (Y * v.X));
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------