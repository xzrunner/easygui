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
		float x = pp.x + rs.frame_padding.x;
		float y = pp.y + rs.frame_padding.y;
		if (gui_st.active_item == id) {
			x += 2;
			y += 2;
		}
		render_text(pt, pp.label, x, y, pp.label_sz.y, rs);
	}

	return pt;
}

}