#include "easygui/ImGui.h"
#include "easygui/GuiState.h"
#include "easygui/Utility.h"
#include "easygui/RenderBuffer.h"
#include "easygui/RenderStyle.h"
#include "easygui/Callback.h"

#include "easygui/Button.h"
#include "easygui/Slider.h"
#include "easygui/Label.h"
#include "easygui/Checkbox.h"

#include <tessellation/Painter.h>

namespace egui
{

bool button(ID_TYPE id, const char* label, float x, float y, float width, float height,
	        GuiState& gui_st, const RenderStyle& rs, RenderBuffer& rb, bool force_draw)
{
	auto sz = Callback::GetLabelSize(label);
	sz.x += rs.frame_padding.x * 2;
	sz.y += rs.frame_padding.y * 2;

	egui::Button btn({ { x, y, sz.x, sz.y, label } });
	auto st = button_update(id, btn, gui_st);

	auto new_st = calc_gui_state(st.event, gui_st, id);
	bool need_render = force_draw || new_st != gui_st || rb.IsRebuilding();

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
      	    GuiState& gui_st, const RenderStyle& rs, RenderBuffer& rb, bool force_draw)
{
	auto st = slider_update(id, { { x, y, height, max }, { *val } }, gui_st);

	auto new_st = calc_gui_state(st.event, gui_st, id);
	bool need_render = force_draw || new_st != gui_st || rb.IsRebuilding() || st.value != *val;
	if (need_render) {
		if (rb.Advance(id, slider_render(id, { { x, y, height, max }, st }, new_st, rs))) {
			gui_st = new_st;
			*val = st.value;
		}
	} else {
		rb.Advance(id);
	}

	return st.event == MouseEvent::DOWN || st.event == MouseEvent::HOLD;
}

void label(ID_TYPE id, const char* text, float x, float y, const RenderStyle& rs, RenderBuffer& rb, bool force_draw)
{
	bool need_render = force_draw || rb.IsRebuilding();
	if (need_render) {
		rb.Advance(id, label_render({ { x, y, text } }, rs));
	} else {
		rb.Advance(id);
	}
}

bool checkbox(ID_TYPE id, const char* label, bool* val, float x, float y, GuiState& gui_st, const RenderStyle& rs, RenderBuffer& rb, bool force_draw)
{
	auto sz = Callback::GetLabelSize(label);
	sz.x += sz.y;
	sz.x += rs.frame_padding.x * 2;
	sz.y += rs.frame_padding.y * 2;

	egui::Checkbox cb({ { x, y, sz.x, sz.y, label }, { *val } });
	auto st = checkbox_update(id, cb, gui_st);

	auto new_st = calc_gui_state(st.event, gui_st, id);
	bool need_render = force_draw || new_st != gui_st || rb.IsRebuilding();

	if (need_render) {
		if (rb.Advance(id, checkbox_render(id, cb, new_st, rs))) {
			gui_st = new_st;
			*val = st.value;
		}
	} else {
		rb.Advance(id);
	}

	return st.event == MouseEvent::DOWN;
}

}