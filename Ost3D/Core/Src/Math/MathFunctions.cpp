// Ost3D::Core - Kasper "OstGeneralen" Esbjornsson - 2025

#include "Public/Math/MathFunctions.h"
#include <cmath>

// ------------------------------------------------------------

float ost::math::Sin(Radians r)
{
    return sinf(r.Value());
}

// ------------------------------------------------------------

float ost::math::Cos(Radians r)
{
    return cosf(r.Value());
}

// ------------------------------------------------------------

float ost::math::Tan(Radians r)
{
    return tanf(r.Value());
}

// ------------------------------------------------------------

float ost::math::SquareRoot(float v)
{
    return sqrtf(v);
}

// ------------------------------------------------------------

bool ost::math::FltEquals(float a, float b, float tolerance)
{
    return a >= (b - tolerance) && a <= (b + tolerance);
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------