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

	draw_rect(pt, btn.props.x + 8, btn.props.y + 8, btn.props.width, btn.props.height, 0xff000000);
	auto& c = rs.colors;
	if (gui_st.hot_item == id)
	{
		if (gui_st.active_item == id) {
			draw_rect(pt, btn.props.x + 2, btn.props.y + 2, btn.props.width, btn.props.height, c[(int)Color::ButtonActive]);
		} else {
			draw_rect(pt, btn.props.x, btn.props.y, btn.props.width, btn.props.height, c[(int)Color::ButtonHovered]);
		}
	}
	else
	{
		draw_rect(pt, btn.props.x, btn.props.y, btn.props.width, btn.props.height, c[(int)Color::Button]);
	}

	return pt;
}

}