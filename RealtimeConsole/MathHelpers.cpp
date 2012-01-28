#include <cmath>
#include "MathHelpers.h"

float Round(const float f)
{
	return (f > 0.f) ? floor(f + 0.5f) : ceil(f - 0.5f);
}

float LERP(const float a, const float b, const float factor)
{
	return b * factor + a * (1 - factor);
}