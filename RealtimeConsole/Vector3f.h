#ifndef VECTOR3F_H
#define VECTOR3F_H

#include "MathHelpers.h"

class Vector3f
{
public:
	Vector3f() : X(0), Y(0), Z(0) {}
	Vector3f(float x, float y, float z) : X(x), Y(y), Z(z) {}
	float X, Y, Z;

	static Vector3f VecLERP(const Vector3f& a, const Vector3f& b, float factor) // a move constructor would be nice here, I suppose
	{
		return Vector3f(LERP(a.X, b.X, factor), LERP(a.Y, b.Y, factor), LERP(a.Z, b.Z, factor));
	}
};

#endif