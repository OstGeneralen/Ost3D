// Ost3D Common - Mat4x4.h

#pragma once
#include <Common/Math/Vector4.h>
#include <Common/Math/RotationUnits.h>

// ------------------------------------------------------------

class Mat3x3;
class Vector3f;

class Mat4x4
{
public:
	constexpr static size_t Rows = 4;
	constexpr static size_t Cols = 4;
	constexpr static size_t Elems = Rows * Cols;
	constexpr static size_t RowStride = sizeof(float) * Cols;
	constexpr static size_t DataSize = RowStride * Rows;
	constexpr static float IdentData[DataSize]{ 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };

private:
	float Data[Elems];
public: // Ctor
	Mat4x4();
	Mat4x4(float data[Elems]);
	Mat4x4(const Mat3x3& transformation, const Vector3f& translation);
	Mat4x4(const Vector3f& translation);

public: // Util
	Vector4f Row1() const;
	Vector4f Row2() const;
	Vector4f Row3() const;
	Vector4f Row4() const;

	Vector4f Col1() const;
	Vector4f Col2() const;
	Vector4f Col3() const;
	Vector4f Col4() const;

public: // Ops
	Mat4x4 operator*(const Mat4x4& m) const;
	Mat4x4 Inverse() const;
	Mat4x4& Transpose();
};

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------