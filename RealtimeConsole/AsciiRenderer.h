#ifndef ASCII_RENDERER_H
#define ASCII_RENDERER_H

#include "Vector3f.h"
#include "Matrix4x4f.h"
#include <cfloat>

class Vector3f;

class AsciiRenderer
{
public:
	AsciiRenderer();
	~AsciiRenderer();

	//also: re-init
	const bool Init(const unsigned int width, const unsigned int height, float fov, float nearClip, float farClip);
	void SetMaterial(const char material);


	const unsigned int GetWidth() const { return m_Width; }
	const unsigned int GetHeight() const { return m_Height; }
	const char * const * const GetColorBuffer() const { return m_ColorBuffer; }

	void DrawTriangle(const Vector3f& p1, const Vector3f& p2, const Vector3f& p3);
	void ClearDepth();
	void ClearColor(const char col = ' ');
	
	void SetModelviewMatrix(const Matrix4x4f& mat) { m_ModelviewMatrix = mat; }
	void ApplyModelviewMatrix(const Matrix4x4f& mat) { m_ModelviewMatrix = m_ModelviewMatrix * mat; }
	
	//todo: move back to private once done testing
	void OrthoDrawTriangle(const Vector3f& p1, const Vector3f& p2, const Vector3f& p3);
	const Vector3f ProcessVector(const Vector3f& vec) const; //applies modelview and projection matrix
private:
	void DrawPixel(const int x, const int y, const float z);
	void CreateBuffers(const unsigned int width, const unsigned int height);

	void SetSize(const unsigned int width, const unsigned int height);


	char m_CurrentMaterial;
	unsigned int m_Width;
	unsigned int m_Height;
	char** m_ColorBuffer; //[y][x]
	float** m_DepthBuffer;
	Matrix4x4f m_ModelviewMatrix;
	Matrix4x4f m_ProjectionMatrix;
	float m_NearClip;
	float m_FarClip;
};

#endif
