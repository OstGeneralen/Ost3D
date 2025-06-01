// Ost3D Common - Mat4x4.cpp

#include "Mat4x4.h"
#include "Mat3x3.h"
#include "Vector3.h"
#include "../Utility/Utilities.h"

// ------------------------------------------------------------
// Ctor
// ------------------------------------------------------------

Math::Mat4x4::Mat4x4()
	: Data()
{
	Util::MemCopy(Data, IdentData, DataSize);
}

Math::Mat4x4::Mat4x4(float data[Elems])
{
	Util::MemCopy(Data, data, DataSize);
}

Math::Mat4x4::Mat4x4(const Mat3x3& transformation, const Vector3f& translation)
	: Mat4x4()
{
	transformation.Row1_CopyTo(&(Data[0]));
	transformation.Row2_CopyTo(&(Data[4]));
	transformation.Row3_CopyTo(&(Data[8]));

	Data[12] = translation.X;
	Data[13] = translation.Y;
	Data[14] = translation.Z;
}

Math::Mat4x4::Mat4x4(const Vector3f& translation)
	: Mat4x4()
{
	Data[12] = translation.X;
	Data[13] = translation.Y;
	Data[14] = translation.Z;
}

// ------------------------------------------------------------
// Util
// ------------------------------------------------------------

Math::Vector4f Math::Mat4x4::Row1() const { return Vector4f{ Data[0], Data[1], Data[2], Data[3]}; }
Math::Vector4f Math::Mat4x4::Row2() const { return Vector4f{ Data[4], Data[5], Data[6], Data[7]}; }
Math::Vector4f Math::Mat4x4::Row3() const { return Vector4f{ Data[8], Data[9], Data[10], Data[11]}; }
Math::Vector4f Math::Mat4x4::Row4() const { return Vector4f{ Data[12], Data[13], Data[14], Data[15]}; }

Math::Vector4f Math::Mat4x4::Col1() const { return Vector4f{ Data[0], Data[4], Data[8], Data[12]}; }
Math::Vector4f Math::Mat4x4::Col2() const { return Vector4f{ Data[1], Data[5], Data[9], Data[13] }; }
Math::Vector4f Math::Mat4x4::Col3() const { return Vector4f{ Data[2], Data[6], Data[10], Data[14] }; }
Math::Vector4f Math::Mat4x4::Col4() const { return Vector4f{ Data[3], Data[7], Data[11], Data[15] }; }

// ------------------------------------------------------------
// Ops
// ------------------------------------------------------------

Math::Mat4x4 Math::Mat4x4::operator*(const Mat4x4& m) const
{
	float d[16]{
			Row1().Dot(m.Col1()), Row1().Dot(m.Col2()), Row1().Dot(m.Col3()), Row1().Dot(m.Col3()),
			Row2().Dot(m.Col1()), Row2().Dot(m.Col2()), Row2().Dot(m.Col3()), Row2().Dot(m.Col3()),
			Row3().Dot(m.Col1()), Row3().Dot(m.Col2()), Row3().Dot(m.Col3()), Row3().Dot(m.Col3()),
			Row4().Dot(m.Col1()), Row4().Dot(m.Col2()), Row4().Dot(m.Col3()), Row4().Dot(m.Col3())
	};
	return Mat4x4(d);
}

Math::Mat4x4& Math::Mat4x4::Transpose()
{
	Util::Swap(Data[1], Data[4]);
	Util::Swap(Data[2], Data[8]);
	Util::Swap(Data[3], Data[12]);
	Util::Swap(Data[7], Data[13]);
	Util::Swap(Data[11], Data[14]);
	Util::Swap(Data[6], Data[9]);
	return *this;
}

Math::Mat4x4 Math::Mat4x4::Inverse() const
{
	// Transformation Inverse
	Mat3x3 transformationInverse = Mat3x3(	Data[0], Data[1], Data[2],
											Data[4], Data[5], Data[6],
											Data[8], Data[9], Data[10]).Inverse();

	return Mat4x4(transformationInverse, Vector3f{ -Data[12], -Data[13], -Data[14] });
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------