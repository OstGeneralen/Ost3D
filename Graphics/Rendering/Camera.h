#pragma once
#include <Common/Math/Mat4x4.h>
#include <Common/Math/Vector3.h>

class Camera
{
public:
	Camera(float aspect, Degrees fovX);
	void SetPosition(const Vector3f& pos);
	void SetForward(const Vector3f& fwd);

	const Mat4x4& GetViewProjection() const;

private:
	Mat4x4 _view;
	Mat4x4 _projection;
	Mat4x4 _vp;

	float _cameraAspect;
	Degrees _cameraFovX;
	Vector3f _cameraLocation;
	Vector3f _cameraForward;
};