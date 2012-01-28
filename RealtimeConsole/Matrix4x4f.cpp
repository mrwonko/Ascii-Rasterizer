#include "Matrix4x4f.h"
#define _USE_MATH_DEFINES //for M_PI
#include <cmath>

Matrix4x4f::Matrix4x4f() :
_11(1), _12(0), _13(0), _14(0),
_21(0), _22(1), _23(0), _24(0),
_31(0), _32(0), _33(1), _34(0),
_41(0), _42(0), _43(0), _44(1)
{
}

Matrix4x4f::Matrix4x4f(float __11, float __12, float __13, float __14,
	float __21, float __22, float __23, float __24,
	float __31, float __32, float __33, float __34,
	float __41, float __42, float __43, float __44) :
_11(__11), _12(__12), _13(__13), _14(__14),
_21(__21), _22(__22), _23(__23), _24(__24),
_31(__31), _32(__32), _33(__33), _34(__34),
_41(__41), _42(__42), _43(__43), _44(__44)
{
}

Matrix4x4f Matrix4x4f::PerspectiveProjection(float fov, float near, float far, float aspect)
{
	//Wikipedia
	if(0)
	{
		return Matrix4x4f(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, (near + far) / near, - far,
			0, 0, 1/near, 0
			);
	}
	
	//http://www.songho.ca/opengl/gl_projectionmatrix.html
	if(0)
	{
		return Matrix4x4f(
			near/aspect, 0, 0, 0,
			0, near, 0, 0,
			0, 0, (near-far)/(far-near), (2*far*near)/(near-far),
			0, 0, -1, 0
			);
	}

	//http://www.opengl.org/sdk/docs/man/xhtml/gluPerspective.xml
	if(0)
	{
		float f = 1.f / tan(fov/2.f);
		return Matrix4x4f(
			f/aspect, 0, 0, 0,
			0, f, 0, 0,
			0, 0, (far+near)/(near-far), (2*far*near)/(near-far),
			0, 0, -1, 0
			);
	}

	if(1)
	{
		//http://www.opengl.org/sdk/docs/man/xhtml/glFrustum.xml + http://nehe.gamedev.net/article/replacement_for_gluperspective/21002/ + brain
		float fH = tan( fov / 360 * M_PI ) * near;
		float fW = fH * aspect;

		return Matrix4x4f(
			near/fW, 0, 0, 0,
			0, near/fH, 0, 0,
			0, 0, - (far + near) / (far - near), 2*far*near/(far-near),
			0, 0, -1, 0
			);
	}
}

Matrix4x4f Matrix4x4f::operator *(const Matrix4x4f& rhs) const
{
	const Matrix4x4f& lhs = *this;
	return Matrix4x4f(
		lhs._11 * rhs._11 + lhs._12 * rhs._21 + lhs._13 * rhs._31 + lhs._14 * rhs._41,
		lhs._11 * rhs._12 + lhs._12 * rhs._22 + lhs._13 * rhs._32 + lhs._14 * rhs._42,
		lhs._11 * rhs._13 + lhs._12 * rhs._23 + lhs._13 * rhs._33 + lhs._14 * rhs._43,
		lhs._11 * rhs._14 + lhs._12 * rhs._24 + lhs._13 * rhs._34 + lhs._14 * rhs._44,

		lhs._21 * rhs._11 + lhs._22 * rhs._21 + lhs._23 * rhs._31 + lhs._24 * rhs._41,
		lhs._21 * rhs._12 + lhs._22 * rhs._22 + lhs._23 * rhs._32 + lhs._24 * rhs._42,
		lhs._21 * rhs._13 + lhs._22 * rhs._23 + lhs._23 * rhs._33 + lhs._24 * rhs._43,
		lhs._21 * rhs._14 + lhs._22 * rhs._24 + lhs._23 * rhs._34 + lhs._24 * rhs._44,

		lhs._31 * rhs._11 + lhs._32 * rhs._21 + lhs._33 * rhs._31 + lhs._34 * rhs._41,
		lhs._31 * rhs._12 + lhs._32 * rhs._22 + lhs._33 * rhs._32 + lhs._34 * rhs._42,
		lhs._31 * rhs._13 + lhs._32 * rhs._23 + lhs._33 * rhs._33 + lhs._34 * rhs._43,
		lhs._31 * rhs._14 + lhs._32 * rhs._24 + lhs._33 * rhs._34 + lhs._34 * rhs._44,

		lhs._41 * rhs._11 + lhs._42 * rhs._21 + lhs._43 * rhs._31 + lhs._44 * rhs._41,
		lhs._41 * rhs._12 + lhs._42 * rhs._22 + lhs._43 * rhs._32 + lhs._44 * rhs._42,
		lhs._41 * rhs._13 + lhs._42 * rhs._23 + lhs._43 * rhs._33 + lhs._44 * rhs._43,
		lhs._41 * rhs._14 + lhs._42 * rhs._24 + lhs._43 * rhs._34 + lhs._44 * rhs._44
		);
}

Matrix4x4f Matrix4x4f::Translation(const Vector3f& translation)
{
	Matrix4x4f mat; //identity
	mat._14 = translation.X;
	mat._24 = translation.Y;
	mat._34 = translation.Z;
	return mat;
}

const Vector3f operator * (const Matrix4x4f& mat, const Vector3f& vec)
{
	return Vector3f(
		vec.X * mat._11 + vec.Y * mat._12 + vec.Z * mat._13 + mat._14,
		vec.X * mat._21 + vec.Y * mat._22 + vec.Z * mat._23 + mat._24,
		vec.X * mat._31 + vec.Y + mat._32 + vec.Z * mat._33 + mat._34
		);
}

const Vector4f operator * (const Matrix4x4f& mat, const Vector4f& vec)
{
	return Vector4f(
		vec.X * mat._11 + vec.Y * mat._12 + vec.Z * mat._13 + mat._14 * vec.W,
		vec.X * mat._21 + vec.Y * mat._22 + vec.Z * mat._23 + mat._24 * vec.W,
		vec.X * mat._31 + vec.Y + mat._32 + vec.Z * mat._33 + mat._34 * vec.W,
		vec.X * mat._41 + vec.Y + mat._42 + vec.Z * mat._43 + mat._44 * vec.W
		);
}