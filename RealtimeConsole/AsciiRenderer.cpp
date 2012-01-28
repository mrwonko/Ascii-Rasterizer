#include "AsciiRenderer.h"
#include <cstring> //NULL
#include <cassert> //assert()
#include "Vector3f.h"
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

const bool AsciiRenderer::Init(const unsigned int width, const unsigned int height)
{
	assert(width > 0);
	assert(height > 0);
	CreateBuffers(width, height);
	return true;
}

void AsciiRenderer::CreateBuffers(const unsigned int width, const unsigned int height)
{
	assert(width != 0);
	assert(height != 0);
	m_Width = width;
	m_Height = height;
	m_ColorBuffer = new char*[height];
	m_DepthBuffer = new short*[height];
	for(unsigned int y = 0; y < height; ++y)
	{
		m_ColorBuffer[y] = new char[width];
		m_DepthBuffer[y] = new short[width];
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
	int end = Round(bot->Y);
	for(int y = Round(top->Y); y <= end; ++y)
	{
		float h = (bot->Y - top->Y); //prevent divide by zero
		if(h == 0.f) h = .1f;
		//calculate position of point on line between top and bottom at "y" height
		Vector3f leftVec = Vector3f::VecLERP(*top, *bot, (y - top->Y) / h);

		h = (mid->Y - top->Y);
		if(h == 0.f) h = .1f;
		//calculate position of point on line between top and middle / middle and bottom (depending on y) at "y" height
		Vector3f rightVec = Vector3f::VecLERP(*top, *mid, (y - top->Y) / h);
		if(h < 1.f) rightVec = *mid; 

		if(y > mid->Y)
		{
			h = (bot->Y - mid->Y);
			if(h == 0.f) h = 1.f;
			rightVec = Vector3f::VecLERP(*mid, *bot, (y - mid->Y) / h);
		}
		//swap em if necessary so we know which one's left
		if(leftVec.X > rightVec.X)
		{
			std::swap(leftVec, rightVec);
		}
		//fill pixels from left to right
		int right = Round(rightVec.X);
		for(int x = Round(leftVec.X); x <= right; ++x)
		{
			DrawPixel(x, y, Round(Vector3f::VecLERP(leftVec, rightVec, (x - leftVec.X) / (rightVec.X - leftVec.X)).Z));
		}
	}
}

void AsciiRenderer::SetMaterial(const char material)
{
	m_CurrentMaterial = material;
}

void AsciiRenderer::DrawPixel(const int x, const int y, const short z)
{
	if(x < 0) return;
	if(x >= m_Width) return;
	if(y < 0) return;
	if(y >= m_Height) return;

	if(z > m_DepthBuffer[y][x]) return;

	m_DepthBuffer[y][x] = z;
	m_ColorBuffer[y][x] = m_CurrentMaterial;
}

void AsciiRenderer::ClearColor()
{
	for(unsigned int y = 0; y < m_Height; ++y)
	{
		for(unsigned int x = 0; x < m_Width; ++x)
		{
			m_ColorBuffer[y][x] = ' ';
		}
	}
}

void AsciiRenderer::ClearDepth()
{
	for(unsigned int y = 0; y < m_Height; ++y)
	{
		for(unsigned int x = 0; x < m_Width; ++x)
		{
			m_DepthBuffer[y][x] = SHRT_MAX;
		}
	}
}
