#include "easygui/ImGui.h"
#include "easygui/Utility.h"
#include "easygui/Callback.h"
#include "easygui/Context.h"

#include "easygui/Button.h"
#include "easygui/Slider.h"
#include "easygui/Label.h"
#include "easygui/Selectable.h"
#include "easygui/Checkbox.h"
#include "easygui/RadioButton.h"
#include "easygui/ArrowButton.h"
#include "easygui/Combo.h"

#include <tessellation/Painter.h>

namespace
{

bool need_render(egui::ID_TYPE id, const egui::Context& ctx, const egui::GuiState& new_st, bool force_draw)
{
	return force_draw || ctx.gui.NeedRender(new_st, id) || ctx.rbuf.IsRebuilding();
}

}

namespace egui
{

bool button(ID_TYPE id, const char* label, float x, float y, float width, float height,
	        Context& ctx, bool force_draw)
{
	bool clicked = false;

	Button btn({ { x, y, label, Callback::GetLabelSize(label) } });
	auto st = button_update(id, btn, ctx);

	bool render_fail = false;
	auto new_st = calc_gui_state(st.event, ctx.gui, id);
	if (btn.state.NeedRender(st) || need_render(id, ctx, new_st, force_draw)) {
		ctx.gui = new_st;
		btn.state = st;
		if (!ctx.rbuf.Advance(id, button_render(id, btn, ctx))) {
			render_fail = true;
		}
	} else {
		ctx.rbuf.Advance(id);
	}

	if (!render_fail) {
		if (st.event == MouseEvent::DOWN) {
			clicked = true;
		}
	}

	return clicked;
}

bool slider(ID_TYPE id, const char* label, float* val, float x, float y, float height, float max, bool verticle,
      	    Context& ctx, bool force_draw)
{
    float old_val = *val;

	Slider sd({ { x, y, height, max, label, Callback::GetLabelSize(label), verticle }, { *val } });
	auto st = slider_update(id, sd, ctx);

	bool render_fail = false;
	auto new_st = calc_gui_state(st.event, ctx.gui, id, true);
	if (sd.state.NeedRender(st) || need_render(id, ctx, new_st, force_draw)) {
		ctx.gui = new_st;
		sd.state = st;
		if (!ctx.rbuf.Advance(id, slider_render(id, sd, ctx))) {
			render_fail = true;
		}
	} else {
		ctx.rbuf.Advance(id);
	}

	if (!render_fail) {
		*val = st.value;
	}

    return *val != old_val;
}

void label(ID_TYPE id, const char* text, float x, float y, Context& ctx, bool force_draw)
{
	bool need_render = force_draw || ctx.rbuf.IsRebuilding();
	if (need_render) {
		ctx.rbuf.Advance(id, label_render({ { x, y, 0, text } }, ctx.style));
	} else {
		ctx.rbuf.Advance(id);
	}
}

bool selectable(ID_TYPE id, const char* label, bool* val, float x, float y, float length, Context& ctx, bool force_draw)
{
	Selectable sel({ { x, y, length, label, Callback::GetLabelSize(label) }, { *val } });
	auto st = selectable_update(id, sel, ctx);

	bool render_fail = false;
	auto new_st = calc_gui_state(st.event, ctx.gui, id);
	if (sel.state.NeedRender(st) || need_render(id, ctx, new_st, force_draw)) {
		ctx.gui = new_st;
		sel.state = st;
		if (!ctx.rbuf.Advance(id, selectable_render(id, sel, ctx))) {
			render_fail = true;
		}
	} else {
		ctx.rbuf.Advance(id);
	}

	if (!render_fail) {
		*val = st.value;
	}

	return *val != sel.state.value;
}

bool checkbox(ID_TYPE id, const char* label, bool* val, float x, float y, Context& ctx, bool force_draw)
{
	Checkbox cbox({ { x, y, label, Callback::GetLabelSize(label) }, { *val } });
	auto st = checkbox_update(id, cbox, ctx);

	bool render_fail = false;
	auto new_st = calc_gui_state(st.event, ctx.gui, id);
	if (cbox.state.NeedRender(st) || need_render(id, ctx, new_st, force_draw)) {
		ctx.gui = new_st;
		cbox.state = st;
		if (!ctx.rbuf.Advance(id, checkbox_render(id, cbox, ctx))) {
			render_fail = true;
		}
	} else {
		ctx.rbuf.Advance(id);
	}

	if (!render_fail) {
		*val = st.value;
	}

	return *val != cbox.state.value;
}

bool radio_button(ID_TYPE id, const char* label, bool val, float x, float y, Context& ctx, bool force_draw)
{
	bool selected = false;

	RadioButton rbtn({ { x, y, val, label, Callback::GetLabelSize(label) } });
	auto st = radio_button_update(id, rbtn, ctx);

	bool render_fail = false;
	auto new_st = calc_gui_state(st.event, ctx.gui, id);
	if (rbtn.state.NeedRender(st) || need_render(id, ctx, new_st, force_draw)) {
		ctx.gui = new_st;
		rbtn.state = st;
		if (!ctx.rbuf.Advance(id, radio_button_render(id, rbtn, ctx))) {
			render_fail = true;
		}
	} else {
		ctx.rbuf.Advance(id);
	}

	if (!render_fail) {
		if (st.event == MouseEvent::DOWN) {
			selected = true;
		}
	}

	return selected;
}

bool arrow_button(ID_TYPE id, Direction dir, float x, float y, float height, bool repeat, Context& ctx, bool force_draw)
{
	bool clicked = false;

	ArrowButton ab({ { x, y, height, dir } });
	auto st = arrow_button_update(id, ab, ctx);

	bool render_fail = false;
	auto new_st = calc_gui_state(st.event, ctx.gui, id);
	if (ab.state.NeedRender(st) || need_render(id, ctx, new_st, force_draw)) {
		ctx.gui = new_st;
		ab.state = st;
		if (!ctx.rbuf.Advance(id, arrow_button_render(id, ab, ctx))) {
			render_fail = true;
		}
	} else {
		ctx.rbuf.Advance(id);
	}

	if (!render_fail)
	{
		if (st.event == MouseEvent::DOWN) {
			clicked = true;
		} else if (repeat && st.event == MouseEvent::HOLD) {
			const float t = ctx.io.GetHoldTime();
			clicked = t < 0;
			if (t <= 0) {
				ctx.io.SetHoldTime(t + ctx.config.key_repeat_delay);
			}
		}
	}

	return clicked;
}

bool combo(ID_TYPE id, const char* label, int* curr_item, const char** const items, int items_count,
	       float x, float y, float length, Context& ctx, bool force_draw)
{
	Combo com({ { x, y, length, label, Callback::GetLabelSize(label), items, items_count },
		        { *curr_item, ctx.gui.GetPopupItem() == id } });
	auto st = combo_update(id, com, ctx);

	bool render_fail = false;
	auto new_st = calc_gui_state(st.event, ctx.gui, st.event_id);
	if (com.state.NeedRender(st) || need_render(st.event_id, ctx, new_st, force_draw)) {
		ctx.gui = new_st;
		com.state = st;
		if (!ctx.rbuf.Advance(id, combo_render(id, com, ctx))) {
			render_fail = true;
		}
	} else {
		ctx.rbuf.Advance(id);
	}

	if (!render_fail)
	{
		*curr_item = st.selected_item;
		if (ctx.gui.GetPopupItem() != id && st.popup) {
			ctx.gui.SetPopupItem(id);
		}
		else if (ctx.gui.GetPopupItem() == id && !st.popup) {
			ctx.gui.SetPopupItem(0);
		}
	}

	return *curr_item != com.state.selected_item;
}

}