// Ost3D::Core - Kasper "OstGeneralen" Esbjornsson - 2025

#include "Public/Math/Angles.h"

#include "Public/Math/Constants.h"

// ------------------------------------------------------------
// DEGREES
// ------------------------------------------------------------

ost::math::Degrees::Degrees() : _v{ 0.0f } {}
ost::math::Degrees::Degrees(float v) : _v{ v } {}

// ------------------------------------------------------------

float ost::math::Degrees::Value() const { return _v; }

// ------------------------------------------------------------

ost::math::Degrees::operator ost::math::Radians() const { return Radians(_v * constants::DegToRad); }

// ------------------------------------------------------------

ost::math::Degrees& ost::math::Degrees::operator=(const Degrees& v)
{
	_v = v._v;
	return *this;
}

// ------------------------------------------------------------

ost::math::Degrees& ost::math::Degrees::operator=(float fv)
{
	_v = fv;
	return *this;
}

// ------------------------------------------------------------

ost::math::Degrees& ost::math::Degrees::operator=(Radians rv)
{
	(*this) = static_cast<Degrees>(rv);
	return *this;
}

// ------------------------------------------------------------
// RADIANS
// ------------------------------------------------------------

ost::math::Radians::Radians()
	: _v(0.0f)
{
}

// ------------------------------------------------------------

ost::math::Radians::Radians(float v)
	: _v(v)
{
}

// ------------------------------------------------------------

float ost::math::Radians::Value() const { return _v; }

// ------------------------------------------------------------

ost::math::Radians::operator ost::math::Degrees() const { return Degrees(_v * constants::RadToDeg); }

// ------------------------------------------------------------

ost::math::Radians& ost::math::Radians::operator=(const Radians& rv)
{
	_v = rv._v;
	return *this;
}

// ------------------------------------------------------------

ost::math::Radians& ost::math::Radians::operator=(float fv)
{
	_v = fv;
	return *this;
}

// ------------------------------------------------------------

ost::math::Radians& ost::math::Radians::operator=(Degrees dv)
{
	(*this) = static_cast<Radians>(dv);
	return *this;
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------