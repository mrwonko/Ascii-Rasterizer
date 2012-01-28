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

class Vector4f //Does not belong here, I could care less
{
public:
	Vector4f() : X(0), Y(0), Z(0), W(1) {}
	Vector4f(const Vector3f& vec) : X(vec.X), Y(vec.Y), Z(vec.Z), W(1) {}
	Vector4f(float x, float y, float z, float w = 1.f) : X(x), Y(y), Z(z), W(w) {}
	float X, Y, Z, W;

	static Vector4f VecLERP(const Vector4f& a, const Vector4f& b, float factor) // a move constructor would be nice here, I suppose
	{
		return Vector4f(LERP(a.X, b.X, factor), LERP(a.Y, b.Y, factor), LERP(a.Z, b.Z, factor), LERP(a.W, b.W, factor));
	}

	const Vector4f operator / (const float f)
	{
		return Vector4f(X/f, Y/f, Z/f, W/f);
	}
};

#endif