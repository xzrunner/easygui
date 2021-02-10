#include "easygui/IOState.h"

namespace egui
{

IOState::IOState()
{
	m_first_pos.MakeInvalid();
}

IOState IOState::FeedEvent(const std::vector<InputEvent>& events) const
{
	IOState st = *this;
	for (auto& e : events)
	{
		switch (e.type)
		{
		case InputType::KEY_DOWN:
		case InputType::KEY_UP:
			st.m_key_code = e.key.code;
			st.m_key_down = e.type == InputType::KEY_DOWN;
			break;
		case InputType::MOUSE_LEFT_DOWN:
		case InputType::MOUSE_LEFT_UP:
		case InputType::MOUSE_RIGHT_DOWN:
		case InputType::MOUSE_RIGHT_UP:
		case InputType::MOUSE_MOVE:
		case InputType::MOUSE_DRAG:
		case InputType::MOUSE_LEFT_DCLICK:
		case InputType::MOUSE_WHEEL_ROTATION:
			st.m_mouse_x = e.mouse.x;
			st.m_mouse_y = e.mouse.y;
			st.m_mouse_dir = e.mouse.direction;
			st.m_mouse_left = (e.type == InputType::MOUSE_LEFT_DOWN) ||
							   (e.type == InputType::MOUSE_LEFT_UP);
			st.m_mouse_down = (e.type == InputType::MOUSE_LEFT_DOWN) ||
							   (e.type == InputType::MOUSE_RIGHT_DOWN) ||
							   (e.type == InputType::MOUSE_DRAG);
			if (!m_mouse_down && st.m_mouse_down) {
				st.m_first_pos.x = e.mouse.x;
				st.m_first_pos.y = e.mouse.y;
			}
			break;
		}
	}
	return st;
}

}