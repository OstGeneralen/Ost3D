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

Unit::Degrees::Degrees() : _v{ 0.0f } {}
Unit::Degrees::Degrees(float v) : _v{v} {}

float Unit::Degrees::Value() const { return _v; }

Unit::Degrees::operator Unit::Radians() const { return Radians(_v * c_degToRad); }

Unit::Degrees& Unit::Degrees::operator=(const Degrees& v)
{
	_v = v._v;
	return *this;
}
Unit::Degrees& Unit::Degrees::operator=(float fv)
{
	_v = fv;
	return *this;
}
Unit::Degrees& Unit::Degrees::operator=(Radians rv)
{
	(*this) = rv;
	return *this;
}

// ------------------------------------------------------------
// RADIANS
// ------------------------------------------------------------

Unit::Radians::Radians()
	: _v(0.0f)
{
}
Unit::Radians::Radians(float v)
	: _v(v)
{
}

float Unit::Radians::Value() const { return _v; }

Unit::Radians::operator Unit::Degrees() const { return Degrees(_v * c_radToDeg); }

Unit::Radians& Unit::Radians::operator=(const Radians& rv)
{
	_v = rv._v;
	return *this;
}
Unit::Radians& Unit::Radians::operator=(float fv)
{
	_v = fv;
	return *this;
}
Unit::Radians& Unit::Radians::operator=(Degrees dv)
{
	(*this) = dv;
	return *this;
}
