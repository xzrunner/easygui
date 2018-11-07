#pragma once

#include "easygui/InputEvent.h"

#include <vector>

namespace egui
{

class IOState
{
public:
	int GetMouseX() const { return m_mouse_x; }
	int GetMouseY() const { return m_mouse_y; }

	bool IsMouseDown() const { return m_mouse_down; }

	float GetHoldTime() const { return m_hold_time; }
	void  SetHoldTime(float time) { m_hold_time = time; }

	IOState FeedEvent(const std::vector<InputEvent>& events) const;

	bool IsMouseClick() const { return m_mouse_click; }

	void Clear() { m_mouse_click = false; }

private:
	int  m_key_code    = 0;
	bool m_key_down    = false;

	int  m_mouse_x     = 0;
	int  m_mouse_y     = 0;
	int  m_mouse_dir   = 0;
	bool m_mouse_left  = false;
	bool m_mouse_down  = false;

	bool m_mouse_click = false;

	float m_hold_time  = 0.0f;

}; // IOState

}