#include "easygui/Utility.h"
#include "easygui/GuiState.h"
#include "easygui/IOState.h"
#include "easygui/Label.h"

#include <tessellation/Painter.h>

namespace egui
{

bool region_hit(const IOState& s, float x, float y, float w, float h)
{
	const int mx = s.GetMouseX();
	const int my = s.GetMouseY();
	return mx >= x && my >= y &&
		   mx < x + w && my < y + h;
}

void draw_rect(tess::Painter& pt, const sm::vec2& min, const sm::vec2& max, uint32_t color)
{
	pt.AddRectFilled(min, max, color);
}

void render_frame(tess::Painter& pt, const sm::vec2& min, const sm::vec2& max, uint32_t color, const RenderStyle& rs, bool border, float rounding)
{
	pt.AddRectFilled(min, max, color);
	auto& border_size = rs.frame_border_size;
	if (border && border_size > 0) {
		pt.AddRect(min + sm::vec2(1, 1), max + sm::vec2(1, 1), rs.colors[(int)Color::BorderShadow], border_size, rounding, tess::CORNER_FLAGS_ALL);
		pt.AddRect(min, max, rs.colors[(int)Color::Border], border_size, rounding, tess::CORNER_FLAGS_ALL);
	}
}

void render_text(ur::Context& ctx, tess::Painter& pt, const char* str, float x, float y, float height, const RenderStyle& rs, bool vert)
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
	pt.AddPainter(label_render(ctx, label, rs));
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
		if (dir == Direction::DOWN) r = -r;
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

	bool hot = false;
	bool hit = false;
	if (region_hit(io_st, x, y, w, h))
	{
		hot = true;
		if (io_st.IsMouseDown()) {
			hit = true;
		}
	}
	else
	{
		return st;
	}

	st = MouseEvent::HOVER;
	if (io_st.IsMouseDown() && hit)
	{
		if (io_st.IsMouseClick()) {
			st = MouseEvent::DOWN;
		} else {
			st = MouseEvent::HOLD;
		}
	}
	else if (!io_st.IsMouseDown() && !hit)
	{
		if (io_st.IsMouseClick()) {
			st = MouseEvent::CLICK;
		} else {
			st = MouseEvent::UP;
		}
	}

	return st;
}

GuiState calc_gui_state(MouseEvent event, const GuiState& gui_st, ID_TYPE id, bool drag)
{
	GuiState new_st = gui_st;
	if (new_st.IsDragLocked() && new_st.GetActiveItem() != id) {
		return new_st;
	}

	if (drag && new_st.IsDragLocked() && new_st.GetActiveItem() == id && event == MouseEvent::UP) {
		new_st.SetDragLocked(false);
	}

	switch (event)
	{
	case MouseEvent::NONE:
		if (new_st.GetHotItem() == id) {
			new_st.SetHotItem(0);
		}
		if (new_st.GetActiveItem() == id &&
			new_st.GetPopupItem() != id) {
			new_st.SetActiveItem(0);
		}
		break;
	case MouseEvent::HOVER:
		new_st.SetHotItem(id);
		if (new_st.GetActiveItem() == id &&
			new_st.GetPopupItem() != id) {
			new_st.SetActiveItem(0);
		}
		break;
	case MouseEvent::HOLD:
	case MouseEvent::DOWN:
		new_st.SetHotItem(id);
		new_st.SetActiveItem(id);
		if (drag) {
			new_st.SetDragLocked(true);
		}
		break;
	case MouseEvent::UP:
		new_st.SetHotItem(id);
		if (new_st.GetActiveItem() == id &&
			new_st.GetPopupItem() != id) {
			new_st.SetActiveItem(0);
		}
		break;
	}
	return new_st;
}

Color get_group3_item_color(ID_TYPE id, const GuiState& gui_st, Color start)
{
	Color col;
	if (gui_st.GetHotItem() == id) {
		if (gui_st.GetActiveItem() == id) {
			col = static_cast<Color>(static_cast<int>(start) + 2);
		} else {
			col = static_cast<Color>(static_cast<int>(start) + 1);
		}
	} else {
		col = start;
	}
	return col;
}

}