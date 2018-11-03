#include "easygui/Utility.h"
#include "easygui/GuiState.h"
#include "easygui/RenderStyle.h"

#include <tessellation/Painter.h>

namespace egui
{

void feed_event(GuiState& state, InputEvent event/*, const sm::vec2& pos, const sm::vec2& scale*/)
{
	switch (event.type)
	{
	case InputType::KEY_DOWN:
	case InputType::KEY_UP:
		state.key_code = event.key.code;
		state.key_down = event.type == InputType::KEY_DOWN;
		break;
	case InputType::MOUSE_LEFT_DOWN:
	case InputType::MOUSE_LEFT_UP:
	case InputType::MOUSE_RIGHT_DOWN:
	case InputType::MOUSE_RIGHT_UP:
	case InputType::MOUSE_MOVE:
	case InputType::MOUSE_DRAG:
	case InputType::MOUSE_LEFT_DCLICK:
	case InputType::MOUSE_WHEEL_ROTATION:
		state.mouse_x = event.mouse.x;
		state.mouse_y = event.mouse.y;
		state.mouse_dir = event.mouse.direction;
		state.mouse_left = (event.type == InputType::MOUSE_LEFT_DOWN) ||
			               (event.type == InputType::MOUSE_LEFT_UP);
		state.mouse_down = (event.type == InputType::MOUSE_LEFT_DOWN) ||
			               (event.type == InputType::MOUSE_RIGHT_DOWN) ||
			               (event.type == InputType::MOUSE_DRAG);
		break;
	}
}

bool region_hit(const GuiState& s, float x, float y, float w, float h)
{
	return s.mouse_x >= x && s.mouse_y >= y &&
		   s.mouse_x < x + w && s.mouse_y < y + h;
}

void draw_rect(tess::Painter& pt, const sm::vec2& min, const sm::vec2& max, uint32_t color)
{
	pt.AddRectFilled(min, max, color);
}

void render_frame(tess::Painter& pt, const sm::vec2& min, const sm::vec2& max, uint32_t color, const RenderStyle& rs, bool border, uint32_t rounding)
{
	pt.AddRectFilled(min, max, color);
	auto& border_size = rs.frame_border_size;
	if (border && border_size > 0) {
		pt.AddRect(min + sm::vec2(1, 1), max + sm::vec2(1, 1), rs.colors[(int)Color::BorderShadow], border_size, rounding/*, ImDrawCornerFlags_All*/);
		pt.AddRect(min, max, rs.colors[(int)Color::Border], border_size, rounding/*, ImDrawCornerFlags_All*/);
	}
}

MouseEvent calc_mouse_event(const GuiState& gui_st, ID_TYPE id, float x, float y, float w, float h)
{
	MouseEvent st = MouseEvent::NONE;

	bool hot    = false;
	bool active = false;
	if (region_hit(gui_st, x, y, w, h))
	{
		hot = true;
		if (gui_st.mouse_down) {
			active = true;
		}
	}
	else
	{
		return st;
	}

	st = MouseEvent::HOVER;
	if (gui_st.mouse_down && active)
	{
		if (gui_st.active_item == id) {
			st = MouseEvent::HOLD;
		} else {
			st = MouseEvent::DOWN;
		}
	}
	else if (!gui_st.mouse_down && gui_st.active_item == id && !active)
	{
		st = MouseEvent::UP;
	}

	return st;
}

GuiState calc_gui_state(MouseEvent event, const GuiState& gui_st, ID_TYPE id)
{
	GuiState new_st = gui_st;
	switch (event)
	{
	case MouseEvent::NONE:
		if (new_st.hot_item == id) {
			new_st.hot_item = 0;
		}
		if (new_st.active_item == id) {
			new_st.active_item = 0;
		}
		break;
	case MouseEvent::HOVER:
		new_st.hot_item = id;
		if (new_st.active_item == id) {
			new_st.active_item = 0;
		}
		break;
	case MouseEvent::HOLD:
	case MouseEvent::DOWN:
		new_st.hot_item    = id;
		new_st.active_item = id;
		break;
	case MouseEvent::UP:
		new_st.hot_item = id;
		if (new_st.active_item == id) {
			new_st.active_item = 0;
		}
		break;
	}
	return new_st;
}

}