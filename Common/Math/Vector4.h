// Ost3D Common - Vector4.h

#pragma once

// ------------------------------------------------------------

class Vector3f;
class Mat4x4;

class Vector4f
{
public:
	float X, Y, Z, W;
public: // Ctor
	Vector4f();
	Vector4f(float xyzw);
	Vector4f(const Vector3f& v);
	Vector4f(const Vector3f& v, float w);
	Vector4f(const Vector4f& v);
	Vector4f(float x, float y, float z, float w);
	Vector4f(Vector4f&& v) noexcept;
	~Vector4f() = default;

public: // Ops
	Vector4f& operator=(const Vector4f& v);
	Vector4f operator*(const Mat4x4& m) const;
	bool operator==(const Vector4f& v) const;

	float Dot(const Vector4f& v) const;
};

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------