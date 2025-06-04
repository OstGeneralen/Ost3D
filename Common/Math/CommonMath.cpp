// ------------------------------------------------------------

#include "CommonMath.h"
#include <cmath>

static float Math::Sin(Radians r)
{
    return sinf(r.Value());
}

static float Math::Cos(Radians r)
{
    return sinf(r.Value());
}

static float Math::SquareRoot(float v)
{
    return sqrtf(v);
}

static bool Math::FltEquals(float a, float b, float tolerance)
{
    return a >= (b - tolerance) && a <= (b + tolerance);
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------
