#ifndef MATH_HELPERS_H
#define MATH_HELPERS_H

float Round(const float f);

float LERP(const float a, const float b, const float factor);

const bool FloatEqual(const float lhs, const float rhs);

//does not belong here but I cba to make a new file
const unsigned long long GetTimeMS();

#endif
