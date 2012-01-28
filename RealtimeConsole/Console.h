#ifndef CONSOLE_H
#define CONSOLE_H

#include <string>

class ConsoleImpl;
struct Event;

class Console
{
public:
	static const bool Init();
	static void Destroy();
	static void Update();
	static const bool PollEvent(Event& ev);
	/** \brief Moves the cursor to the given position
		\note 0, 0 is the upper left corner **/
	static void GotoXY(const unsigned int x, const unsigned int y);
	static void Write(const std::string& text);
	static unsigned int GetWidth();
	static unsigned int GetHeight();
private:
	static ConsoleImpl* m_Impl;
};

#endif
