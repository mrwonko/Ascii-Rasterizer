#include "AsciiRenderer.h"
#include "MathHelpers.h"
#include <cstring> //NULL
#include <cassert> //assert()
#include <algorithm>

AsciiRenderer::AsciiRenderer() :
m_CurrentMaterial(' '),
m_Width(0),
m_Height(0),
m_ColorBuffer(NULL),
m_DepthBuffer(NULL)
{
}

AsciiRenderer::~AsciiRenderer()
{
}

const bool AsciiRenderer::Init(const unsigned int width, const unsigned int height, float fov, float nearClip, float farClip)
{
	if(m_ColorBuffer != NULL)
	{
		for(unsigned int y = 0; y < m_Height; ++y)
		{
			delete[] m_ColorBuffer[y];
		}
		delete[] m_ColorBuffer;
	}
	if(m_DepthBuffer != NULL)
	{
		for(unsigned int y = 0; y < m_Height; ++y)
		{
			delete[] m_DepthBuffer[y];
		}
		delete[] m_DepthBuffer;
	}
	assert(width > 0);
	assert(height > 0);
	m_ProjectionMatrix = Matrix4x4f::PerspectiveProjection(fov, nearClip, farClip, float(width)/float(height));
	m_NearClip = nearClip;
	m_FarClip = farClip;
	CreateBuffers(width, height); //calls clearDepth which needs farClip, thus last
	return true;
}

void AsciiRenderer::CreateBuffers(const unsigned int width, const unsigned int height)
{
	assert(width != 0);
	assert(height != 0);
	m_Width = width;
	m_Height = height;
	m_ColorBuffer = new char*[height];
	m_DepthBuffer = new float*[height];
	for(unsigned int y = 0; y < height; ++y)
	{
		m_ColorBuffer[y] = new char[width];
		m_DepthBuffer[y] = new float[width];
	}
	ClearColor();
	ClearDepth();
}

void AsciiRenderer::OrthoDrawTriangle(const Vector3f& p1, const Vector3f& p2, const Vector3f& p3)
{
	//order points by Y-Position
	const Vector3f* top = &p1;
	const Vector3f* mid = &p2;
	const Vector3f* bot = &p3;
	if(mid->Y < top->Y)
	{
		std::swap(top, mid);
	}
	if(bot->Y < top->Y)
	{
		std::swap(top, bot);
	}
	if(bot->Y < mid->Y)
	{
		std::swap(bot, mid);
	}

	//For every line occupied by this triangle:
	int start = int(Round(top->Y));
	int end = int(Round(bot->Y));
	if(start < 0) start = 0; //no need to draw outside the screen
	if(end >= m_Height) end = m_Height - 1;
	for(int y = start; y <= end; ++y)
	{
		//left position - between top & bot
		Vector3f leftVec, rightVec;
		if(FloatEqual(Round(bot->Y), Round(top->Y))) //both at the same height? take the left one then.
		{
			leftVec = top->X < bot->X ? *top : *bot;
		}
		else //otherwise: LERP
		{
			leftVec = Vector3f::VecLERP(*top, *bot, (y - top->Y) / (bot->Y - top->Y));
		}

		//right position
		const Vector3f* rTop = top;
		const Vector3f* rBot = mid;
		if(y > mid->Y)
		{
			rTop = mid;
			rBot = bot;
		}
		//same height? Use right one
		if(FloatEqual(Round(rTop->Y), Round(rBot->Y)))
		{
			rightVec = rTop->X > rBot->X ? *rTop : *rBot;
		}
		else //LERP
		{
			rightVec = Vector3f::VecLERP(*rTop, *rBot, (y - rTop->Y) / (rBot->Y - rTop->Y));
		}
		//swap em if necessary so we know which one's left
		if(leftVec.X > rightVec.X)
		{
			std::swap(leftVec, rightVec);
		}
		//fill pixels from left to right
		int left = int(Round(leftVec.X));
		int right = int(Round(rightVec.X));
		if(left < 0) left = 0; //no need to draw outside the screen
		if(right >= m_Width) right = m_Width - 1;
		int width = right - left;
		for(int x = 0; x <= width; ++x)
		{
			DrawPixel(left + x, y, LERP(leftVec.Z, rightVec.Z, (FloatEqual(rightVec.X - leftVec.X, 0.f) ? 0.f : ((x + left - leftVec.X)  / (rightVec.X - leftVec.X)))));
		}
	}
}

void AsciiRenderer::SetMaterial(const char material)
{
	m_CurrentMaterial = material;
}

void AsciiRenderer::DrawPixel(const int x, const int y, const float z)
{
	if(x < 0) return;
	if(x >= m_Width) return;
	if(y < 0) return;
	if(y >= m_Height) return;

	if(z < m_NearClip) //near
	{
		return;
	}

	//if(z > m_DepthBuffer[m_Height - 1 - y][x])
	if(z > m_DepthBuffer[y][x])
	{
		return;
	}

	//m_DepthBuffer[m_Height - 1 - y][x] = z; // y is up!
	//m_ColorBuffer[m_Height - 1 - y][x] = m_CurrentMaterial;
	m_DepthBuffer[y][x] = z;
	m_ColorBuffer[y][x] = m_CurrentMaterial;
}

void AsciiRenderer::ClearColor(const char col)
{
	for(unsigned int y = 0; y < m_Height; ++y)
	{
		for(unsigned int x = 0; x < m_Width; ++x)
		{
			m_ColorBuffer[y][x] = col;
		}
	}
}

void AsciiRenderer::ClearDepth()
{
	for(unsigned int y = 0; y < m_Height; ++y)
	{
		for(unsigned int x = 0; x < m_Width; ++x)
		{
			m_DepthBuffer[y][x] = m_FarClip;
		}
	}
}

const Vector3f AsciiRenderer::ProcessVector(const Vector3f& vec) const
{
	Vector4f out = (m_ProjectionMatrix) * (m_ModelviewMatrix * Vector4f(vec));
	out = out / out.W; // for perspective 
	return Vector3f(
		m_Width/2.f*(1+out.X),
		m_Height/2.f * (1 + out.Y),
		(m_FarClip - m_NearClip)/2.f*out.Z + (m_FarClip + m_NearClip)/2.f
		//out.Z //let's keep these in range -1 .. 1
		);
}

void AsciiRenderer::DrawTriangle(const Vector3f& p1, const Vector3f& p2, const Vector3f& p3)
{
	OrthoDrawTriangle(ProcessVector(p1), ProcessVector(p2), ProcessVector(p3));
}
