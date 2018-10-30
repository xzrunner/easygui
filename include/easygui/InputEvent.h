#pragma once

namespace egui
{

enum class InputType
{
	KEY_DOWN = 0,
	KEY_UP,

	MOUSE_LEFT_DOWN,
	MOUSE_LEFT_UP,
	MOUSE_RIGHT_DOWN,
	MOUSE_RIGHT_UP,
	MOUSE_MOVE,
	MOUSE_DRAG,
	MOUSE_LEFT_DCLICK,
	MOUSE_WHEEL_ROTATION,
};

struct InputEvent
{
	InputEvent(InputType _type, int code)
		: type(type)
	{
		key.code = code;
	}
	InputEvent(InputType type, int x, int y, int dircetion = 0)
		: type(type)
	{
		mouse.x = x;
		mouse.y = y;
		mouse.direction = dircetion;
	}

	InputType type;

	struct KeyEvent
	{
		int code;
	};

	struct MouseEvent
	{
		int x = 0;
		int y = 0;
		int direction = 0;
	};

	union
	{
		KeyEvent   key;
		MouseEvent mouse;
	};

}; // InputEvent

}