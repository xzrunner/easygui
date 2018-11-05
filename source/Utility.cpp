#include "easygui/Utility.h"
#include "easygui/GuiState.h"
#include "easygui/IOState.h"
#include "easygui/Label.h"

#include <tessellation/Painter.h>

namespace egui
{

void feed_event(IOState& state, InputEvent event/*, const sm::vec2& pos, const sm::vec2& scale*/)
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

bool region_hit(const IOState& s, float x, float y, float w, float h)
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

void render_text(tess::Painter& pt, const char* str, float x, float y, float height, const RenderStyle& rs, bool vert)
{
	float angle = 0;
	if (vert)
	{
		angle = - SM_PI * 0.5f;
		x += height * 0.5f;
	}
	else
	{
		y += height * 0.5f;
	}
	Label label({ x, y, angle, str });
	pt.AddPainter(label_render(label, rs));
}

void render_arrow(tess::Painter& pt, const sm::vec2& min, float height, Direction dir, uint32_t color)
{
	const auto center = min + sm::vec2(height * 0.50f, height * 0.50f);

	sm::vec2 a, b, c;
	float r = height * 0.4f;
	switch (dir)
	{
	case Direction::UP:
	case Direction::DOWN:
		if (dir == Direction::UP) r = -r;
		a = sm::vec2(+0.000f, +0.750f) * r;
		b = sm::vec2(-0.866f, -0.750f) * r;
		c = sm::vec2(+0.866f, -0.750f) * r;
		break;
	case Direction::LEFT:
	case Direction::RIGHT:
		if (dir == Direction::LEFT) r = -r;
		a = sm::vec2(+0.750f, +0.000f) * r;
		b = sm::vec2(-0.750f, +0.866f) * r;
		c = sm::vec2(-0.750f, -0.866f) * r;
		break;
	case Direction::NONE:
		assert(0);
		break;
	}

	pt.AddTriangleFilled(center + a, center + b, center + c, color);
}

MouseEvent calc_mouse_event(const GuiState& gui_st, const IOState& io_st, ID_TYPE id, float x, float y, float w, float h)
{
	MouseEvent st = MouseEvent::NONE;

	bool hot    = false;
	bool active = false;
	if (region_hit(io_st, x, y, w, h))
	{
		hot = true;
		if (io_st.mouse_down) {
			active = true;
		}
	}
	else
	{
		return st;
	}

	st = MouseEvent::HOVER;
	if (io_st.mouse_down && active)
	{
		if (gui_st.active_item == id) {
			st = MouseEvent::HOLD;
		} else {
			st = MouseEvent::DOWN;
		}
	}
	else if (!io_st.mouse_down && gui_st.active_item == id && !active)
	{
		st = MouseEvent::UP;
	}

	return st;
}

GuiState calc_gui_state(MouseEvent event, const GuiState& gui_st, ID_TYPE id, bool drag)
{
	GuiState new_st = gui_st;
	if (new_st.drag_locked && new_st.active_item != id) {
		return new_st;
	}

	if (drag && new_st.drag_locked && new_st.active_item == id && event == MouseEvent::UP) {
		new_st.drag_locked = false;
	}

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
		if (drag) {
			new_st.drag_locked = true;
		}
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

Color get_frame_bg_color(ID_TYPE id, const GuiState& gui_st)
{
	Color col;
	if (gui_st.hot_item == id) {
		if (gui_st.active_item == id) {
			col = Color::FrameBgActive;
		} else {
			col = Color::FrameBgHovered;
		}
	} else {
		col = Color::FrameBg;
	}
	return col;
}

Color get_button_color(ID_TYPE id, const GuiState& gui_st)
{
	Color col;
	if (gui_st.hot_item == id) {
		if (gui_st.active_item == id) {
			col = Color::ButtonActive;
		} else {
			col = Color::ButtonHovered;
		}
	} else {
		col = Color::Button;
	}
	return col;
}

}