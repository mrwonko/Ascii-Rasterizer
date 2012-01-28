#include "Console.h"
#include <cassert>
#include <cstring> //NULL
#include "ConsoleImpl.h"

ConsoleImpl* Console::m_Impl = NULL;

const bool Console::Init()
{
	m_Impl = new ConsoleImpl();
	if(m_Impl->Init())
	{
		return true;
	}
	delete m_Impl;
	return false;
}

void Console::Destroy()
{
	assert(m_Impl);
	m_Impl->Destroy();
	delete m_Impl;
}

void Console::Update()
{
	assert(m_Impl);
	m_Impl->Update();
}

const bool Console::PollEvent(Event& ev)
{
	assert(m_Impl);
	return m_Impl->PollEvent(ev);
}

void Console::GotoXY(const unsigned int x, const unsigned int y)
{
	assert(m_Impl);
	m_Impl->GotoXY(x, y);
}

void Console::Write(const std::string& text)
{
	assert(m_Impl);
	m_Impl->Write(text);
}

unsigned int Console::GetWidth()
{
	assert(m_Impl);
	return m_Impl->GetConsoleWidth();
}

unsigned int Console::GetHeight()
{
	assert(m_Impl);
	return m_Impl->GetConsoleHeight();
}
