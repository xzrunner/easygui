#include "easygui/Button.h"
#include "easygui/GuiState.h"
#include "easygui/RenderStyle.h"
#include "easygui/Callback.h"
#include "easygui/CompStorage.h"
#include "easygui/Utility.h"

#include <tessellation/Painter.h>

namespace egui
{

Button::State button_update(ID_TYPE id, const Button& btn, const GuiState& gui_st)
{
	Button::State st = btn.state;
	st.event = calc_mouse_event(gui_st, id, btn.props.x, btn.props.y, btn.props.width, btn.props.height);
	return st;
}

tess::Painter button_render(ID_TYPE id, const Button& btn, const GuiState& gui_st, const RenderStyle& rs)
{
	tess::Painter pt;

	Color col;
	float x = btn.props.x;
	float y = btn.props.y;
	if (gui_st.hot_item == id)
	{
		if (gui_st.active_item == id) {
			x += 2;
			y += 2;
			col = Color::ButtonActive;
		} else {
			col = Color::ButtonHovered;
		}
	}
	else
	{
		col = Color::Button;
	}
	draw_rect(pt, x, y, btn.props.width, btn.props.height, rs.colors[(int)col]);

	if (btn.props.label)
	{
		sm::vec2 pos;
		pos.x = btn.props.x + rs.frame_padding.x;
		pos.y = btn.props.y + rs.frame_padding.y + btn.props.height * 0.5f - 2;	// fixme: layout
		if (gui_st.active_item == id) {
			pos.x += 2;
			pos.y += 2;
		}
		Callback::DrawLabel(btn.props.label, pos, rs.colors[(int)Color::Text], pt);
	}

	return pt;
}

}