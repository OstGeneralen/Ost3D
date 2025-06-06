// ------------------------------------------------------------

#include "CommonMath.h"
#include <cmath>

using namespace ost;

float math::Sin(Radians r)
{
    return sinf(r.Value());
}

float math::Cos(Radians r)
{
    return cosf(r.Value());
}

float math::Tan(Radians r)
{
    return tanf(r.Value());
}

float math::SquareRoot(float v)
{
    return sqrtf(v);
}

bool math::FltEquals(float a, float b, float tolerance)
{
    return a >= (b - tolerance) && a <= (b + tolerance);
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------
