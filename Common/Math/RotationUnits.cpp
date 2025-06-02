#include "RotationUnits.h"
#include "CommonMath.h"

// ------------------------------------------------------------
// CONSTANTS
// ------------------------------------------------------------

constexpr float c_degToRad = Math::Constants::PI / 180.0f;
constexpr float c_radToDeg = 180.0f / Math::Constants::PI;

// ------------------------------------------------------------
// DEGREES
// ------------------------------------------------------------

Degrees::Degrees() : _v{ 0.0f } {}
Degrees::Degrees(float v) : _v{v} {}

float Degrees::Value() const { return _v; }

Degrees::operator Radians() const { return Radians(_v * c_degToRad); }

Degrees& Degrees::operator=(const Degrees& v)
{
	_v = v._v;
	return *this;
}
Degrees& Degrees::operator=(float fv)
{
	_v = fv;
	return *this;
}
Degrees& Degrees::operator=(Radians rv)
{
	(*this) = rv;
	return *this;
}

// ------------------------------------------------------------
// RADIANS
// ------------------------------------------------------------

Radians::Radians()
	: _v(0.0f)
{
}
Radians::Radians(float v)
	: _v(v)
{
}

float Radians::Value() const { return _v; }

Radians::operator Degrees() const { return Degrees(_v * c_radToDeg); }

Radians& Radians::operator=(const Radians& rv)
{
	_v = rv._v;
	return *this;
}
Radians& Radians::operator=(float fv)
{
	_v = fv;
	return *this;
}
Radians& Radians::operator=(Degrees dv)
{
	(*this) = dv;
	return *this;
}
