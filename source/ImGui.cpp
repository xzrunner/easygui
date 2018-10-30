#include "easygui/ImGui.h"
#include "easygui/GuiState.h"
#include "easygui/Utility.h"
#include "easygui/RenderBuffer.h"

#include "easygui/Button.h"
#include "easygui/Slider.h"

#include <tessellation/Painter.h>

namespace egui
{

bool button(ID_TYPE id, float x, float y, float width, float height,
	        GuiState& gui_st, const RenderStyle& rs, RenderBuffer& rb)
{
	egui::Button btn({ { x, y, width, height } });
	auto st = button_update(id, btn, gui_st);

	auto new_st = calc_gui_state(st.event, gui_st, id);
	bool need_render = new_st != gui_st || rb.IsRebuilding();

	if (need_render) {
		if (rb.Advance(id, button_render(id, btn, new_st, rs))) {
			gui_st = new_st;
		}
	} else {
		rb.Advance(id);
	}

	return st.event == MouseEvent::DOWN;
}

bool slider(ID_TYPE id, float* val, float x, float y, float height, float max,
      	    GuiState& gui_st, const RenderStyle& rs, RenderBuffer& rb)
{
	auto st = slider_update(id, { { x, y, height, max }, { *val } }, gui_st);

	auto new_st = calc_gui_state(st.event, gui_st, id);
	bool need_render = new_st != gui_st || rb.IsRebuilding() || st.value != *val;
	if (need_render) {
		if (rb.Advance(id, slider_render(id, { { x, y, height, max }, st }, new_st, rs))) {
			gui_st = new_st;
		}
	} else {
		rb.Advance(id);
	}

	*val = st.value;

	return st.event == MouseEvent::DOWN || st.event == MouseEvent::HOLD;
}

}