// Ost3D::Core - Kasper "OstGeneralen" Esbjornsson - 2025

#pragma once
#include "Ost3D/Core/Public/Math/Angles.h"
#include "Ost3D/Core/Public/Math/Constants.h"

// ------------------------------------------------------------

namespace ost
{
	namespace math
	{
		extern float Sin(Radians r);
		extern float Cos(Radians r);
		extern float Tan(Radians r);

		extern float SquareRoot(float v);

		extern bool FltEquals(float a, float b, float tolerance = constants::FLT_Small);

		// Safe equals operation that use FltEquals for float
		template<typename T> static bool EqualsT(T a, T b) { return a == b; }
		template<> static bool EqualsT<float>(float a, float b) { return FltEquals(a, b); }
	}
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------
