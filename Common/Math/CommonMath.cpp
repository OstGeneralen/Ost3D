// ------------------------------------------------------------

#include "CommonMath.h"
#include <cmath>

float Math::Sin(Unit::Radians r)
{
    return sinf(r.Value());
}

float Math::Cos(Unit::Radians r)
{
    return sinf(r.Value());
}

float Math::SquareRoot(float v)
{
    return sqrtf(v);
}

bool Math::FltEquals(float a, float b, float tolerance)
{
    return a >= (b - tolerance) && a <= (b + tolerance);
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------
