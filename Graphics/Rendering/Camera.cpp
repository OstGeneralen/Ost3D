#include "Camera.h"

Camera::Camera(float aspect, Degrees fovX)
	: _view()
	, _projection()
	, _cameraAspect(aspect)
	, _cameraFovX(fovX)
	, _cameraLocation(0)
	, _cameraForward(0,0,1)
{
	_view = Mat4x4::CreateView(_cameraLocation, _cameraLocation + _cameraForward, Vector3f(0, 1, 0));
	_projection = Mat4x4::CreatePerspective(_cameraFovX, _cameraAspect, 0.1f, 10.0f);
	_vp = _view * _projection;
}

void Camera::SetPosition(const Vector3f& pos)
{
	_cameraLocation = pos;
	_view = Mat4x4::CreateView(_cameraLocation, _cameraLocation + _cameraForward, Vector3f(0, 1, 0));
	_vp = _view * _projection;
}

void Camera::SetForward(const Vector3f& fwd)
{
	_cameraForward = fwd;
	_view = Mat4x4::CreateView(_cameraLocation, _cameraLocation + _cameraForward, Vector3f(0, 1, 0));
	_vp = _view * _projection;
}

const Mat4x4& Camera::GetViewProjection() const
{
	return _vp;
}
