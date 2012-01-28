#include "Console.h"
#include "Event.h"
#include <iostream>
#include <sstream>

#include "AsciiRenderer.h"
#include "Vector3f.h"
#include "Windows.h"
#include "Matrix4x4f.h"

#include "MathHelpers.h"
//#include <cassert>

int main(int argc, char** argv)
{
	//Matrix4x4f mat = Matrix4x4f::PerspectiveProjection(90, 0.1, 100, 1.f/1.f);
	//assert((mat * Matrix4x4f()) == mat); //works :)

	if(1) //renderer test
	{
		AsciiRenderer renderer;
		if(!renderer.Init(80, 40, 90, 0.1f, 100.f))
		{
			std::cout<<"Could not init!"<<std::endl;
			return 0;
		}

		//not necessary here, but keep them in mind
		renderer.ClearColor();
		renderer.ClearDepth();

		//move 10 units back
		renderer.ApplyModelviewMatrix(Matrix4x4f::Translation(Vector3f(0, 0, -10)));
		
		renderer.SetMaterial('#');
		renderer.DrawTriangle(Vector3f(5, -5, 0), Vector3f(5, 5, 0), Vector3f(5, 5, -10));
		renderer.SetMaterial('o');
		renderer.DrawTriangle(Vector3f(5, -5, 0), Vector3f(5, -5, -10), Vector3f(5, 5, -10));
		
		renderer.SetMaterial('+');
		renderer.DrawTriangle(Vector3f(-15, -5, 0), Vector3f(-15, 5, 0), Vector3f(-5, 5, -10));
		renderer.SetMaterial('c');
		renderer.DrawTriangle(Vector3f(-15, -5, 0), Vector3f(-5, -5, -10), Vector3f(-5, 5, -10));
	
		
		renderer.SetMaterial('x');
		//renderer.OrthoDrawTriangle(Vector3f(14, 0, 1), Vector3f(19, 0, 1), Vector3f(19, 5, 1));
		

		//renderer.DrawTriangle(Vector3f(2, 2, -5), Vector3f(2, 12, -5), Vector3f(12, 12, -5));
		//renderer.DrawTriangle(Vector3f(2, 2, -5), Vector3f(12, 2, -5), Vector3f(12, 12, -5));

		std::stringstream ss;
		char const * const * const colBuf = renderer.GetColorBuffer();
		for(unsigned int y = 0; y < renderer.GetHeight(); ++y)
		{
			ss << std::string(colBuf[y], renderer.GetWidth()) << "\n";
		}
		fputs(ss.str().c_str(), stdout);

		system("pause");

		return 0;
	}

	if(!Console::Init())
	{
		std::cerr<<"Could not initialize Console handler!"<<std::endl;
		return 0;
	}

	Console::GotoXY(0, 0);
	unsigned int w = Console::GetWidth();
	unsigned int h = Console::GetHeight();

	static const bool BUFFER = true;
	std::stringstream ss;

	for(unsigned int y = 0; y < h - 1; ++y) //cannot write on last line due to linebreak at EOL
	{
		if(BUFFER)
		{
			ss << y;
		}
		else
		{
			std::cout << y;
		}
		//Console::GotoXY(5, y);
		for(unsigned int x = (y > 9 ? 2 : 1); x < w; ++x)
		{
			//Console::Write("x");
			if(BUFFER)
			{
				ss << "x";
			}
			else
			{
				std::cout<<"x";
			}
		}
	}
	if(BUFFER)
	{
		if(false) // cout 100% buffered
		{
			char* buffer = new char[h * w];
			std::cout.rdbuf()->pubsetbuf(buffer, h * w);
			std::cout << ss.str() << std::flush;
			std::cout.rdbuf()->pubsetbuf(0, 0); //attention: is now unbuffered!
			delete buffer;
		}
		else if(false) // cout 0% buffered - characterwise?
		{
			std::cout.rdbuf()->pubsetbuf(0, 0);
			std::cout << ss.str() << std::flush;
		}
		else // fputs
		{
			Console::Write(ss.str());
			std::cout<<std::flush;
		}
	}
	Console::GotoXY(0, 0);

	bool running = true;

	long long counter = 0;
	while(running)
	{
		Event ev;
		while(Console::PollEvent(ev))
		{
			if(ev.Type == KeyPressed && ev.Key.Code == KEYCODE_ESCAPE)
			{
				running = false;
			}
			/*
			else if(ev.Type == Resized)
			{
				std::cout<<"Resized to "<<ev.Resize.SizeX<<", "<<ev.Resize.SizeY<<std::endl;
			}
			else if(ev.Type == MouseButtonPressed)
			{
				std::cout<<"Mouse button " << ev.MouseButton.Button << " pressed" << std::endl;
			}
			else if(ev.Type == MouseButtonReleased)
			{
				std::cout<<"Mouse button " << ev.MouseButton.Button << " released" << std::endl;
			}
			else if(ev.Type == MouseWheelMoved)
			{
				std::cout<< "Mouse wheel moved by " << ev.MouseWheel.Delta << std::endl;
			}
			else if(ev.Type == KeyPressed)
			{
				std::cout << "Key " << int(ev.Key.Code) << " pressed" << std::endl;
			}
			else if(ev.Type == KeyReleased)
			{
				std::cout << "Key " << int(ev.Key.Code) << " released" << std::endl;
			}
			else if(ev.Type == MouseMoved)
			{
				std::cout << "Mouse moved by " << ev.MouseMove.X << ", " << ev.MouseMove.Y << std::endl;
			}
			*/
		}
		Console::Update();

		//Console::GotoXY(0, 0);
		//std::cout<<++counter;
	}
	Console::Destroy();
	return 0;
}