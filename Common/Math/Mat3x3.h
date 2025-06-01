// Ost3D Common - Mat3x3.h

#pragma once
#include "Common/Math/RotationUnits.h"

namespace Math
{
	class Vector3f;

	class Mat3x3
	{
	public:
		constexpr static size_t Rows = 3;
		constexpr static size_t Cols = 3;
		constexpr static size_t Elems = Rows * Cols;
		constexpr static size_t RowStride = sizeof(float) * Cols;
		constexpr static size_t DataSize = RowStride * Rows;
		constexpr static float IdentData[DataSize]{ 1,0,0, 0,1,0, 0,0,1 };

	private:
		float Data[Elems];
	public: // Statics
		const static Mat3x3 Identity;

		static Mat3x3 XRotation(Unit::Radians r);
		static Mat3x3 YRotation(Unit::Radians r);
		static Mat3x3 ZRotation(Unit::Radians r);

	public: // Ctor
		Mat3x3();
		Mat3x3(float row1[Cols], float row2[Cols], float row3[Cols]);
		Mat3x3(float m11, float m21, float m31, float m12, float m22, float m32, float m13, float m23, float m33);
		Mat3x3(float const data[Elems]);

	public: // Util
		Vector3f Row1() const;
		Vector3f Row2() const;
		Vector3f Row3() const;

		void Row1_CopyTo(float* dst) const;
		void Row2_CopyTo(float* dst) const;
		void Row3_CopyTo(float* dst) const;

		Vector3f Col1() const;
		Vector3f Col2() const;
		Vector3f Col3() const;

	public: // Ops
		Mat3x3& operator*=(const Mat3x3& m);
		Mat3x3 operator*(const Mat3x3& m) const;
		Mat3x3 Inverse() const;
		Mat3x3 Transpose() const;
	
	private: 
		Mat3x3 CofactorMatrix() const;
		
		float Determinant() const;

		float MinorDeterminant11() const;
		float MinorDeterminant21() const;
		float MinorDeterminant31() const;
		float MinorDeterminant12() const;
		float MinorDeterminant22() const;
		float MinorDeterminant32() const;
		float MinorDeterminant13() const;
		float MinorDeterminant23() const;
		float MinorDeterminant33() const;
	};
}