// Ost3D Common - CommonMath.h

#pragma once
#include <Common/Math/RotationUnits.h>

// ------------------------------------------------------------

namespace Math
{
	namespace Constants
	{
		constexpr float FLT_Small = 0.0001f;
		constexpr float PI = 3.14159f;
	}

	extern float Sin(Unit::Radians r);
	extern float Cos(Unit::Radians r);

	extern float SquareRoot(float v);

	extern bool FltEquals(float a, float b, float tolerance = Constants::FLT_Small);
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------