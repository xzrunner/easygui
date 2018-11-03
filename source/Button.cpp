#include "easygui/Button.h"
#include "easygui/GuiState.h"
#include "easygui/RenderStyle.h"
#include "easygui/Callback.h"
#include "easygui/CompStorage.h"
#include "easygui/Utility.h"
#include "easygui/Label.h"

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

	auto& pp = btn.props;

	Color col;
	float x = pp.x;
	float y = pp.y;
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
	render_frame(pt, { x, y }, { x + pp.width, y + pp.height }, rs.colors[(int)col], rs);

	if (pp.label)
	{
		sm::vec2 pos;
		pos.x = pp.x + rs.frame_padding.x;
		pos.y = pp.y + rs.frame_padding.y + pp.height * 0.5f - 2;	// fixme: layout
		if (gui_st.active_item == id) {
			pos.x += 2;
			pos.y += 2;
		}

		Label label({ pos.x, pos.y, pp.label });
		pt.AddPainter(label_render(label, rs));
	}

	return pt;
}

}