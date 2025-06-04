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

	extern float Sin(Radians r);
	extern float Cos(Radians r);

	extern float SquareRoot(float v);

	extern bool FltEquals(float a, float b, float tolerance = Constants::FLT_Small);

	// Safe equals operation that use FltEquals for float
	template<typename T> static bool EqualsT(T a, T b) { return a == b; }
	template<> static bool EqualsT<float>(float a, float b) { return FltEquals(a, b); }
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------