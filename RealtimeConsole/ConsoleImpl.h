#ifndef CONSOLE_IMPL_H
#define CONSOLE_IMPL_H

#ifdef _WIN32
#include <Windows.h>
#include <deque>
#endif
#include <string>

struct Event;

class ConsoleImpl
{
public:
	ConsoleImpl();
	const bool Init();
	void Destroy();
	void Update();
	const bool PollEvent(Event& ev);
	void GotoXY(const unsigned int x, const unsigned int y);
	void Write(const std::string& text);
	unsigned int GetConsoleWidth();
	unsigned int GetConsoleHeight();

private:
#ifdef _WIN32
	HWND m_HWnd;
	HINSTANCE m_HInst;
	HANDLE m_OutputHandle;
	HANDLE m_InputHandle;
	DWORD m_PreviousInputMode;
	DWORD m_PreviousMouseButtons;
	std::deque<POINT> m_MouseMoveQueue;
	POINT m_PreviousMousePos;
#endif
};

#endif
