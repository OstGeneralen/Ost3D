// Ost3D Common - Mat4x4.cpp

#include "Mat4x4.h"
#include "Mat3x3.h"
#include "Vector3.h"
#include "../Utility/Utilities.h"

// ------------------------------------------------------------
// Ctor
// ------------------------------------------------------------

Mat4x4::Mat4x4()
	: Data()
{
	Util::MemCopy(Data, IdentData, DataSize);
}

Mat4x4::Mat4x4(float data[Elems])
{
	Util::MemCopy(Data, data, DataSize);
}

Mat4x4::Mat4x4(const Mat3x3& transformation, const Vector3f& translation)
	: Mat4x4()
{
	transformation.Row1_CopyTo(&(Data[0]));
	transformation.Row2_CopyTo(&(Data[4]));
	transformation.Row3_CopyTo(&(Data[8]));

	Data[12] = translation.X;
	Data[13] = translation.Y;
	Data[14] = translation.Z;
}

Mat4x4::Mat4x4(const Vector3f& translation)
	: Mat4x4()
{
	Data[12] = translation.X;
	Data[13] = translation.Y;
	Data[14] = translation.Z;
}

// ------------------------------------------------------------
// Util
// ------------------------------------------------------------

Vector4f Mat4x4::Row1() const { return Vector4f{ Data[0], Data[1], Data[2], Data[3]}; }
Vector4f Mat4x4::Row2() const { return Vector4f{ Data[4], Data[5], Data[6], Data[7]}; }
Vector4f Mat4x4::Row3() const { return Vector4f{ Data[8], Data[9], Data[10], Data[11]}; }
Vector4f Mat4x4::Row4() const { return Vector4f{ Data[12], Data[13], Data[14], Data[15]}; }

Vector4f Mat4x4::Col1() const { return Vector4f{ Data[0], Data[4], Data[8], Data[12]}; }
Vector4f Mat4x4::Col2() const { return Vector4f{ Data[1], Data[5], Data[9], Data[13] }; }
Vector4f Mat4x4::Col3() const { return Vector4f{ Data[2], Data[6], Data[10], Data[14] }; }
Vector4f Mat4x4::Col4() const { return Vector4f{ Data[3], Data[7], Data[11], Data[15] }; }

// ------------------------------------------------------------
// Ops
// ------------------------------------------------------------

Mat4x4 Mat4x4::operator*(const Mat4x4& m) const
{
	float d[16]{
			Row1().Dot(m.Col1()), Row1().Dot(m.Col2()), Row1().Dot(m.Col3()), Row1().Dot(m.Col3()),
			Row2().Dot(m.Col1()), Row2().Dot(m.Col2()), Row2().Dot(m.Col3()), Row2().Dot(m.Col3()),
			Row3().Dot(m.Col1()), Row3().Dot(m.Col2()), Row3().Dot(m.Col3()), Row3().Dot(m.Col3()),
			Row4().Dot(m.Col1()), Row4().Dot(m.Col2()), Row4().Dot(m.Col3()), Row4().Dot(m.Col3())
	};
	return Mat4x4(d);
}

Mat4x4& Mat4x4::Transpose()
{
	Util::Swap(Data[1], Data[4]);
	Util::Swap(Data[2], Data[8]);
	Util::Swap(Data[3], Data[12]);
	Util::Swap(Data[7], Data[13]);
	Util::Swap(Data[11], Data[14]);
	Util::Swap(Data[6], Data[9]);
	return *this;
}

Mat4x4 Mat4x4::CreateView(const Vector3f eyeLocation, const Vector3f targetLocation, const Vector3f& up)
{
	const Vector3f axis_z = (targetLocation - eyeLocation).Normalized();
	const Vector3f axis_x = (up.Cross(axis_z)).Normalized();
	const Vector3f axis_y = up;

	const float x_dot = eyeLocation.Dot(axis_x);
	const float y_dot = eyeLocation.Dot(axis_y);
	const float z_dot = eyeLocation.Dot(axis_z);

	float m[]{
		axis_x.X,	axis_y.X,	axis_z.X,	0,
		axis_x.Y,	axis_y.Y,	axis_z.Y,	0,
		axis_x.Z,	axis_y.Z,	axis_z.Z,	0,
		-x_dot,		-y_dot,		-z_dot,		1
	};

	return Mat4x4(m);
}

Mat4x4 Mat4x4::CreatePerspective(Radians fovX, float aspect, float near, float far)
{
	float scale_x = 1.0f / Math::Tan(fovX.Value() / 2.0f);
	float scale_y = scale_x * aspect;

	float f = far / (far - near);
	float n = (-near * far) / (far - near);

	float m[]{
		scale_x,	0,			0,			0,
		0,			scale_y,	0,			0,
		0,			0,			f,			1,
		0,			0,			n,			0
	};

	return Mat4x4(m);
}

Mat4x4 Mat4x4::Inverse() const
{
	// Transformation Inverse
	Mat3x3 transformationInverse = Mat3x3(	Data[0], Data[1], Data[2],
											Data[4], Data[5], Data[6],
											Data[8], Data[9], Data[10]).Inverse();

	return Mat4x4(transformationInverse, Vector3f{ -Data[12], -Data[13], -Data[14] });
}

Mat4x4 Mat4x4::GetTransposed() const
{
	return Mat4x4(*this).Transpose();
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------