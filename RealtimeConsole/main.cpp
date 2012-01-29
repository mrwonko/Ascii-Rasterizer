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
		if(!renderer.Init(80, 40, 90, 0.1f, 100.f) || !Console::Init()) //console init is required for GotoXY even though it's actually not
		//if(!renderer.Init(80, 40, 90, 0.1f, 25.f) || !Console::Init()) //console init is required for GotoXY even though it's actually not
		{
			std::cout<<"Could not init!"<<std::endl;
			return 0;
		}

		if(0) // z value test
		{
			Vector3f vecs[] =
			{
				Vector3f(0, 0, -1.f),
				Vector3f(0, 0, 0.1f),
				Vector3f(0, 0, 1.f),
				Vector3f(0, 0, 25.f),
				Vector3f(0, 0, 50.f),
				Vector3f(0, 0, 100.f),
				Vector3f(0, 0, 150.f),
				Vector3f(0, 0, 0.f),
			};
			for(unsigned int i = 0; i < 8; ++i)
			{
				std::cout<<vecs[i].Z << " -> " << renderer.ProcessVector(vecs[i]).Z << std::endl;
			}
			system("pause");
			return 0;
		}


		while(true)
		{
			unsigned long long ms_start = GetTimeMS();
			renderer.ClearColor();
			renderer.ClearDepth();

			//move 10 units back
			renderer.SetModelviewMatrix(Matrix4x4f::Translation(Vector3f(0, 0, -20)));
			renderer.ApplyModelviewMatrix(Matrix4x4f::SimpleRotation(0.f, (ms_start % (5000)) * (360.f / 5000.f) / 180.f * 3.1415f));
		
			//renderer.SetMaterial('#');
			//renderer.DrawTriangle(Vector3f(-2, -1, 0), Vector3f(2, -1, 0), Vector3f(0, 2, 0));

			
			renderer.SetMaterial('#');
			renderer.DrawTriangle(Vector3f(5, -5, 0), Vector3f(5, 5, 0), Vector3f(5, 5, -10));
			renderer.SetMaterial('o');
			renderer.DrawTriangle(Vector3f(5, -5, 0), Vector3f(5, -5, -10), Vector3f(5, 5, -10));
		
			renderer.SetMaterial('+');
			renderer.DrawTriangle(Vector3f(-15, -5, 0), Vector3f(-15, 5, 0), Vector3f(-5, 5, -10));
			renderer.SetMaterial('c');
			renderer.DrawTriangle(Vector3f(-15, -5, 0), Vector3f(-5, -5, -10), Vector3f(-5, 5, -10));
			

			std::stringstream ss;
			char const * const * const colBuf = renderer.GetColorBuffer();
			for(unsigned int y = 0; y < renderer.GetHeight(); ++y)
			{
				ss << std::string(colBuf[y], renderer.GetWidth());// << "\n"; //if width is not correct
			}
			Console::GotoXY(0, 0);
			fputs(ss.str().c_str(), stdout);
		}

		if(0)
		{
			renderer.SetMaterial('T'); //for Test
			renderer.OrthoDrawTriangle(Vector3f(5, 5, 2), Vector3f(5, 15, 2), Vector3f(-5, 10, 2));
			renderer.SetMaterial('U');
			renderer.OrthoDrawTriangle(Vector3f(10, 5, 2), Vector3f(10, 15, 2), Vector3f(15, 10, 2));
			renderer.SetMaterial('V');
			renderer.OrthoDrawTriangle(Vector3f(25, 10, 2), Vector3f(25, 20, 2), Vector3f(15, 15, 2));
			//renderer.DrawTriangle(Vector3f(-5, -5, -5), Vector3f(5, -5, -5), Vector3f(0, -5, -150));

			std::stringstream ss;
			char const * const * const colBuf = renderer.GetColorBuffer();
			for(unsigned int y = 0; y < renderer.GetHeight(); ++y)
			{
				ss << std::string(colBuf[y], renderer.GetWidth());// << "\n"; //if width is not correct
			}
			Console::GotoXY(0, 0);
			fputs(ss.str().c_str(), stdout);
			system("pause");
			return 0;
		}

		renderer.SetMaterial('1');
		renderer.DrawTriangle(Vector3f(-2, 0, -4), Vector3f(0, 0, -4), Vector3f(0, 2, -4));
		renderer.SetMaterial('2');
		renderer.DrawTriangle(Vector3f(-2, 0.5f, -3.9f), Vector3f(0, 0.5f, -3.9f), Vector3f(0, 2.5f, -3.9f));

		renderer.SetMaterial('3');
		renderer.DrawTriangle(Vector3f(-2, 0, 4), Vector3f(0, 0, 4), Vector3f(0, 2, 4));
		
		renderer.SetMaterial('.');
		renderer.DrawTriangle(Vector3f(-10, 0, -10), Vector3f(-10, 0, -10), Vector3f(10, 0, -10));

		renderer.SetMaterial('4');
		renderer.SetModelviewMatrix(Matrix4x4f::Translation(Vector3f(-2, 0, -10)));
		renderer.DrawTriangle(Vector3f(-2, -1, 0), Vector3f(0, -1, 0), Vector3f(0, 2, 0));
		
		std::stringstream ss;
		char const * const * const colBuf = renderer.GetColorBuffer();
		for(unsigned int y = 0; y < renderer.GetHeight(); ++y)
		{
			ss << std::string(colBuf[y], renderer.GetWidth());// << "\n"; //if width is not correct
		}
		Console::GotoXY(0, 0);
		fputs(ss.str().c_str(), stdout);
		
		//unsigned long long ms_end = GetTimeMS();
		//std::cout<<"Drawtime: " << ms_end - ms_start << " ms" <<std::endl;

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