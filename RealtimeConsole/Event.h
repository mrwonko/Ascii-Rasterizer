#ifndef EVENT_H
#define EVENT_H

#include "KeyCode.h"

// Mouse button pressed/released info
struct MouseButtonEvent
{
	unsigned int Button;
};

// Mouse moved info - deltas
struct MouseMoveEvent
{
	int X;
	int Y;
};

// Window resize info
struct ResizeEvent
{
	unsigned int SizeX;
	unsigned int SizeY;
};

// Key pressed/released info
struct KeyEvent
{
	KeyCode Code;
};

struct MouseWheelEvent
{
	int Delta;
};

enum EventType
{
	//MouseButtonEvent
	MouseButtonPressed,
	MouseButtonReleased,
	//KeyEvent
	KeyPressed,
	KeyReleased,
	//MouseMoveEvent
	MouseMoved,
	//ResizeEvent
	Resized,
	//MouseWHeelEvent
	MouseWheelMoved
};

struct Event
{
	EventType Type;
	union
	{
		MouseButtonEvent MouseButton;
		MouseMoveEvent MouseMove;
		KeyEvent Key;
		ResizeEvent Resize;
		MouseWheelEvent MouseWheel;
	};
};

#endif
