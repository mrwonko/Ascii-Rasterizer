#include "ConsoleImpl.h"
#include <cstdio>
#include "Event.h"

#ifdef _WIN32
namespace
{
	KeyCode KeyConvert(const WORD winKey)
	{
		if((winKey >= 'A' && winKey <= 'Z') || (winKey >= '1' && winKey <= '9') || winKey == '0')
		{
			return KeyCode(winKey);
		}
		if(winKey == VK_RETURN) return KEYCODE_RETURN;
		if(winKey == VK_ESCAPE) return KEYCODE_ESCAPE;
		if(winKey == VK_TAB) return KEYCODE_TAB;
		if(winKey == VK_SPACE) return KEYCODE_SPACE;
		return KEYCODE_NONE;
	}
}
#endif

ConsoleImpl::ConsoleImpl()
#ifdef _WIN32
	:
	m_HWnd(NULL),
	m_HInst(NULL),
	m_PreviousMouseButtons(0)
#endif
{
}

const bool ConsoleImpl::Init()
{
#ifdef _WIN32
	m_HWnd = GetConsoleWindow();
	if( m_HWnd == NULL) return false;

	m_HInst = GetModuleHandle(0);
	if(m_HInst == NULL) return false;

	m_OutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if(m_OutputHandle == INVALID_HANDLE_VALUE || m_OutputHandle == NULL) return false;

	m_InputHandle = GetStdHandle(STD_INPUT_HANDLE);
	if(m_InputHandle == INVALID_HANDLE_VALUE || m_InputHandle == NULL) return false;
	
	if (!GetConsoleMode(m_InputHandle, &m_PreviousInputMode))
	{
		return false;
	}

	if(!SetConsoleMode(m_InputHandle, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT))
	{
		return false;
	}
	SetCursorPos(100, 100);
	if(!GetCursorPos(&m_PreviousMousePos)) return false;
	//TODO: Hide Cursor
	return true;
#else
#error Console not implemented on this OS
#endif
}

void ConsoleImpl::Destroy()
{
#ifdef _WIN32
	SetConsoleMode(m_InputHandle, m_PreviousInputMode);
	//TODO: Show Cursor again (needs to be hidden first)
#endif
}

void ConsoleImpl::Update()
{
	POINT pos;
	if(!GetCursorPos(&pos)) return;
	POINT delta;
	delta.x = pos.x - m_PreviousMousePos.x;
	delta.y = pos.y - m_PreviousMousePos.y;
	if(delta.x != 0 || delta.y != 0)
	{
		m_MouseMoveQueue.push_back(delta);
	}
	if(0)
	{
		SetCursorPos(m_PreviousMousePos.x, m_PreviousMousePos.y);
	}
	else
	{
		m_PreviousMousePos = pos;
	}
}

const bool ConsoleImpl::PollEvent(Event& ev)
{
#ifdef _WIN32
	if(!m_MouseMoveQueue.empty())
	{
		ev.Type = MouseMoved;
		ev.MouseMove.X = m_MouseMoveQueue.front().x;
		ev.MouseMove.Y = m_MouseMoveQueue.front().y;
		m_MouseMoveQueue.pop_front();
		return true;
	}
	INPUT_RECORD record;
	//for(unsigned int i = 0; i < 100; ++i) //no more than 100 events per frame - I seem to get stuck in an infinite loop here?
	DWORD numEvents;
	if(!GetNumberOfConsoleInputEvents(m_InputHandle, &numEvents))
	{
		return false;
	}
	while(numEvents > 0)
	{
		DWORD numRead;
		if(!ReadConsoleInput(m_InputHandle, &record, 1, &numRead))
		{
			return false;
		}
		if(numRead == 0)
		{
			return false;
		}
		switch(record.EventType)
		{
		case KEY_EVENT:
			{
				KeyCode code = KeyConvert(record.Event.KeyEvent.wVirtualKeyCode);
				if(code == KEYCODE_NONE)
				{
					break;
				}
				ev.Key.Code = code;
				ev.Type = (record.Event.KeyEvent.bKeyDown) ? KeyPressed : KeyReleased;
				return true;
			}
		case MOUSE_EVENT:
			{
				if(record.Event.MouseEvent.dwEventFlags == 0 || record.Event.MouseEvent.dwEventFlags == DOUBLE_CLICK)
				{
					DWORD change = m_PreviousMouseButtons ^ record.Event.MouseEvent.dwButtonState;
					if(change == FROM_LEFT_1ST_BUTTON_PRESSED)
					{
						ev.MouseButton.Button = 0;
					}
					else if(change == RIGHTMOST_BUTTON_PRESSED)
					{
						ev.MouseButton.Button = 1;
					}
					else if(change == FROM_LEFT_2ND_BUTTON_PRESSED)
					{
						ev.MouseButton.Button = 2;
					}
					else if(change == FROM_LEFT_3RD_BUTTON_PRESSED)
					{
						ev.MouseButton.Button = 3;
					}
					else if(change == FROM_LEFT_4TH_BUTTON_PRESSED)
					{
						ev.MouseButton.Button = 4;
					}
					else
					{
						break; //invalid key
					}
					m_PreviousMouseButtons = record.Event.MouseEvent.dwButtonState;
					ev.Type = (m_PreviousMouseButtons & change) ? MouseButtonPressed : MouseButtonReleased;
					return true;
				}
				if(record.Event.MouseEvent.dwEventFlags == MOUSE_WHEELED)
				{
					ev.Type = MouseWheelMoved;
					ev.MouseWheel.Delta = record.Event.MouseEvent.dwButtonState;
					return true;
				}
				//move events here are per-cell, not per-pixel - I don't use them.
				break;
			}
		case WINDOW_BUFFER_SIZE_EVENT:
			{
				ev.Type = Resized;
				ev.Resize.SizeX = record.Event.WindowBufferSizeEvent.dwSize.X;
				ev.Resize.SizeY = record.Event.WindowBufferSizeEvent.dwSize.Y;
				return true;
			}
		case FOCUS_EVENT:
			{
				//should be ignored
				break;
			}
		default:
			{
				//ignored
				break;
			}
		} //switch

		// event was ignored - let's see how many are left.
		if(!GetNumberOfConsoleInputEvents(m_InputHandle, &numEvents))
		{
			return false;
		}
	}
	return false;
#else
#error not implemented
#endif
}

void ConsoleImpl::GotoXY(unsigned int x, unsigned int y)
{
#ifdef _WIN32
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(m_OutputHandle, coord);
#else
#error Not implemented
#endif
}

void ConsoleImpl::Write(const std::string& text)
{
	fputs(text.c_str(), stdout); //like puts except without the newline
}

unsigned int ConsoleImpl::GetConsoleWidth()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	if(!GetConsoleScreenBufferInfo(m_OutputHandle, &info)) return 0;
	return info.dwSize.X;
}

unsigned int ConsoleImpl::GetConsoleHeight()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	if(!GetConsoleScreenBufferInfo(m_OutputHandle, &info)) return 0;
	return info.dwSize.Y;
}
