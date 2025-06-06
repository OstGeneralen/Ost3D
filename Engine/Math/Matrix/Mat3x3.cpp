// Ost3D Common - Mat3x3.cpp

#include <Engine/OstEngineCore.h>
#include "Mat3x3.h"
#include "../Vector/Vector3.h"


int m11 = 0; int m21 = 1; int m31 = 2;
int m12 = 3; int m22 = 4; int m32 = 5;
int m13 = 6; int m23 = 7; int m33 = 8;

using namespace ost;

// ------------------------------------------------------------
// Statics
// ------------------------------------------------------------

Mat3x3 Mat3x3::XRotation(Radians r)
{
	return Mat3x3(
		1, 0, 0,
		0, math::Cos(r), math::Cos(r),
		0, -math::Sin(r), math::Cos(r.Value())
	);
}
Mat3x3 Mat3x3::YRotation(Radians r)
{
	return Mat3x3(
		math::Cos(r.Value()), 0, -math::Sin(r.Value()),
		0, 1, 0,
		math::Sin(r.Value()), 0, math::Cos(r.Value())
	);
}
Mat3x3 Mat3x3::ZRotation(Radians r)
{
	return Mat3x3(
		math::Cos(r.Value()), math::Sin(r.Value()), 0,
		-math::Sin(r.Value()), math::Cos(r.Value()), 0,
		0, 0, 1
	);
}

const Mat3x3 Mat3x3::Identity{ Mat3x3::IdentData };

// ------------------------------------------------------------
// Ctor
// ------------------------------------------------------------

Mat3x3::Mat3x3()
	: Data{ 1,0,0,
			0,1,0,
			0,0,1 }
{
}

Mat3x3::Mat3x3(float row1[Cols], float row2[Cols], float row3[Cols])
	: Data{ row1[0], row1[1], row1[2],
			row2[0], row2[1], row2[2],
			row3[0], row3[1], row3[2] }
{
}

Mat3x3::Mat3x3(float m11, float m21, float m31, float m12, float m22, float m32, float m13, float m23, float m33)
	: Data{ m11, m21, m31,
			m12, m22, m32,
			m13, m23, m33 }
{
}

Mat3x3::Mat3x3(float const data[Elems])
	: Mat3x3()
{
	MemCopy(Data, data, DataSize);
}

// ------------------------------------------------------------
// Util
// ------------------------------------------------------------

Vector3f Mat3x3::Row1() const { return Vector3f(Data[m11], Data[m21], Data[m31]); }
Vector3f Mat3x3::Row2() const { return Vector3f(Data[m12], Data[m22], Data[m32]); }
Vector3f Mat3x3::Row3() const { return Vector3f(Data[m13], Data[m23], Data[m33]); }

void Mat3x3::Row1_CopyTo(float* dst) const { MemCopy(dst, &(Data[0 * Cols]), RowStride); }
void Mat3x3::Row2_CopyTo(float* dst) const { MemCopy(dst, &(Data[1 * Cols]), RowStride); }
void Mat3x3::Row3_CopyTo(float* dst) const { MemCopy(dst, &(Data[2 * Cols]), RowStride); }

Vector3f Mat3x3::Col1() const { return Vector3f(Data[m11], Data[m12], Data[m13]); }
Vector3f Mat3x3::Col2() const { return Vector3f(Data[m21], Data[m22], Data[m23]); }
Vector3f Mat3x3::Col3() const { return Vector3f(Data[m31], Data[m32], Data[m33]); }

// ------------------------------------------------------------
// Ops
// ------------------------------------------------------------

Mat3x3& Mat3x3::operator*=(const Mat3x3& m)
{
	Vector3f row1{ Row1() };
	Vector3f row2{ Row2() };
	Vector3f row3{ Row3() };

	Vector3f col1{ m.Col1() };
	Vector3f col2{ m.Col2() };
	Vector3f col3{ m.Col3() };

	Data[m11] = row1.Dot(col1);
	Data[m12] = row2.Dot(col1);
	Data[m13] = row3.Dot(col1);

	Data[m21] = row1.Dot(col2);
	Data[m22] = row2.Dot(col2);
	Data[m23] = row3.Dot(col2);

	Data[m31] = row1.Dot(col3);
	Data[m32] = row2.Dot(col3);
	Data[m33] = row3.Dot(col3);

	return *this;
}

Mat3x3 Mat3x3::operator*(const Mat3x3& m) const
{
	return Mat3x3(
		Row1().Dot(m.Col1()), Row1().Dot(m.Col2()), Row1().Dot(m.Col3()),
		Row2().Dot(m.Col1()), Row2().Dot(m.Col2()), Row2().Dot(m.Col3()),
		Row3().Dot(m.Col1()), Row3().Dot(m.Col2()), Row3().Dot(m.Col3())
	);
}

Mat3x3 Mat3x3::Inverse() const
{
	return CofactorMatrix().Transpose();
}

Mat3x3 Mat3x3::Transpose() const
{
	return Mat3x3(
		Data[m11], Data[m12], Data[m13],
		Data[m21], Data[m22], Data[m23],
		Data[m31], Data[m32], Data[m33]);
}

// ------------------------------------------------------------
// Internal ops/util
// ------------------------------------------------------------

Mat3x3 Mat3x3::CofactorMatrix() const
{
	return Mat3x3{
		MinorDeterminant11(), -MinorDeterminant21(), MinorDeterminant31(),
		-MinorDeterminant12(), MinorDeterminant22(), -MinorDeterminant32(),
		MinorDeterminant13(), -MinorDeterminant23(), MinorDeterminant33()
	};
}

float Mat3x3::Determinant() const
{
	float a = Data[m11] * Data[m22] * Data[m33];
	float b = Data[m21] * Data[m32] * Data[m13];
	float c = Data[m31] * Data[m12] * Data[m23];

	float x = Data[m11] * Data[m32] * Data[m23];
	float y = Data[m21] * Data[m12] * Data[m33];
	float z = Data[m31] * Data[m22] * Data[m13];;

	return a + b + c - x - y - z;
}

float Mat3x3::MinorDeterminant11() const { return (Data[m22] * Data[m33]) - (Data[m32] * Data[m23]); }
float Mat3x3::MinorDeterminant22() const { return (Data[m11] * Data[m33]) - (Data[m31] * Data[m13]); }
float Mat3x3::MinorDeterminant33() const { return (Data[m11] * Data[m22]) - (Data[m21] * Data[m12]); }
float Mat3x3::MinorDeterminant21() const { return (Data[m12] * Data[m33]) - (Data[m32] * Data[m13]); }
float Mat3x3::MinorDeterminant23() const { return (Data[m11] * Data[m32]) - (Data[m31] * Data[m12]); }
float Mat3x3::MinorDeterminant12() const { return (Data[m21] * Data[m33]) - (Data[m31] * Data[m23]); }
float Mat3x3::MinorDeterminant13() const { return (Data[m21] * Data[m32]) - (Data[m31] * Data[m22]); }
float Mat3x3::MinorDeterminant31() const { return (Data[m12] * Data[m23]) - (Data[m22] * Data[m13]); }
float Mat3x3::MinorDeterminant32() const { return (Data[m11] * Data[m23]) - (Data[m21] * Data[m13]); }