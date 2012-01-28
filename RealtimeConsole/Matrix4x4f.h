#ifndef MATRIX_4X4F_H
#define MATRIX_4X4F_H

#include "Vector3f.h"

class Matrix4x4f
{
public:
	Matrix4x4f(); //identity
	Matrix4x4f(float __11, float __12, float __13, float __14,
		float __21, float __22, float __23, float __24,
		float __31, float __32, float __33, float __34,
		float __41, float __42, float __43, float __44);

	float _11, _12, _13, _14,
		_21, _22, _23, _24,
		_31, _32, _33, _34,
		_41, _42, _43, _44;

	static Matrix4x4f PerspectiveProjection(float fov, float near, float far, float aspect);
	static Matrix4x4f Translation(const Vector3f& translation);

	Matrix4x4f operator *(const Matrix4x4f& rhs) const;

	const bool operator ==(const Matrix4x4f& rhs) const
	{
		return FloatEqual(_11, rhs._11) && FloatEqual(_12, rhs._12) && FloatEqual(_13, rhs._13) && FloatEqual(_14, rhs._14) && 
			FloatEqual(_21, rhs._21) && FloatEqual(_22, rhs._22) && FloatEqual(_23, rhs._23) && FloatEqual(_24, rhs._24) && 
			FloatEqual(_31, rhs._31) && FloatEqual(_32, rhs._32) && FloatEqual(_33, rhs._33) && FloatEqual(_34, rhs._34) && 
			FloatEqual(_41, rhs._41) && FloatEqual(_42, rhs._42) && FloatEqual(_43, rhs._43) && FloatEqual(_44, rhs._44);
	}
};

const Vector3f operator * (const Matrix4x4f& lhs, const Vector3f& rhs);
const Vector4f operator * (const Matrix4x4f& lhs, const Vector4f& rhs);

#endif
