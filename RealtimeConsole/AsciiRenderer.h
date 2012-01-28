#ifndef ASCII_RENDERER_H
#define ASCII_RENDERER_H

class Vector3f;

class AsciiRenderer
{
public:
	AsciiRenderer();
	~AsciiRenderer();

	const bool Init(const unsigned int width, const unsigned int height);
	void SetMaterial(const char material);


	const unsigned int GetWidth() const { return m_Width; }
	const unsigned int GetHeight() const { return m_Height; }
	const char * const * const GetColorBuffer() const { return m_ColorBuffer; }

	void ClearDepth(); //todo: implement!
	void ClearColor(); //todo: implement!
	
	void OrthoDrawTriangle(const Vector3f& p1, const Vector3f& p2, const Vector3f& p3); //todo: move to private once tested
private:
	void DrawPixel(const int x, const int y, const short z);

	void CreateBuffers(const unsigned int width, const unsigned int height);

	char m_CurrentMaterial;
	unsigned int m_Width;
	unsigned int m_Height;
	char** m_ColorBuffer; //[y][x]
	short** m_DepthBuffer;
};

#endif
