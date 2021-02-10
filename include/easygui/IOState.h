#pragma once

#include "easygui/InputEvent.h"

#include <SM_Vector.h>

#include <vector>

namespace egui
{

class IOState
{
public:
	IOState();

	int GetMouseX() const { return m_mouse_x; }
	int GetMouseY() const { return m_mouse_y; }

	bool IsMouseDown() const { return m_mouse_down; }

	float GetHoldTime() const { return m_hold_time; }
	void SetHoldTime(float time) { m_hold_time = time; }

	IOState FeedEvent(const std::vector<InputEvent>& events) const;

	int GetClickTime() const { return m_click_time; }
	void SetClickTime(int click_time) const { m_click_time = click_time; }

	auto& GetFirstPos() const { return m_first_pos; }
	void ResetFirstPos() const { m_first_pos.MakeInvalid(); }

private:
	int  m_key_code    = 0;
	bool m_key_down    = false;

	int  m_mouse_x     = 0;
	int  m_mouse_y     = 0;
	int  m_mouse_dir   = 0;
	bool m_mouse_left  = false;
	bool m_mouse_down  = false;

	float m_hold_time = 0;

	mutable int m_click_time = 0;
	mutable sm::ivec2 m_first_pos;

}; // IOState

}