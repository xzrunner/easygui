#include "easygui/ImGui.h"
#include "easygui/Utility.h"
#include "easygui/Callback.h"
#include "easygui/Context.h"

#include "easygui/Button.h"
#include "easygui/Slider.h"
#include "easygui/Label.h"
#include "easygui/Checkbox.h"
#include "easygui/RadioButton.h"
#include "easygui/ArrowButton.h"

#include <tessellation/Painter.h>

namespace egui
{

bool button(ID_TYPE id, const char* label, float x, float y, float width, float height,
	        Context& ctx, bool force_draw)
{
	egui::Button btn({ { x, y, label, Callback::GetLabelSize(label) } });
	auto st = button_update(id, btn, ctx);

	auto new_st = calc_gui_state(st.event, ctx.gui, id);
	bool need_render = force_draw || new_st != ctx.gui || ctx.rbuf.IsRebuilding();

	if (need_render) {
		auto old_st = ctx.gui;
		ctx.gui = new_st;
		if (!ctx.rbuf.Advance(id, button_render(id, btn, ctx))) {
			ctx.gui = old_st;
		}
	} else {
		ctx.rbuf.Advance(id);
	}

	return st.event == MouseEvent::DOWN;
}

bool slider(ID_TYPE id, const char* label, float* val, float x, float y, float height, float max, bool verticle,
      	    Context& ctx, bool force_draw)
{
	const float old_val = *val;

	auto label_sz = Callback::GetLabelSize(label);
	auto st = slider_update(id, { { x, y, height, max, label, label_sz, verticle }, { *val } }, ctx);

	auto new_st = calc_gui_state(st.event, ctx.gui, id, true);
	bool need_render = force_draw || new_st != ctx.gui || ctx.rbuf.IsRebuilding() || st.value != *val;

	if (need_render) {
		auto old_st = ctx.gui;
		ctx.gui = new_st;
		if (ctx.rbuf.Advance(id, slider_render(id, { { x, y, height, max, label, label_sz, verticle }, st }, ctx))) {
			*val = st.value;
		} else {
			ctx.gui = old_st;
		}
	} else {
		ctx.rbuf.Advance(id);
	}

	return *val != old_val;;
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

bool checkbox(ID_TYPE id, const char* label, bool* val, float x, float y, Context& ctx, bool force_draw)
{
	egui::Checkbox cb({ { x, y, label, Callback::GetLabelSize(label) }, { *val } });
	auto st = checkbox_update(id, cb, ctx);

	auto new_st = calc_gui_state(st.event, ctx.gui, id);
	bool need_render = force_draw || new_st != ctx.gui || ctx.rbuf.IsRebuilding();

	if (need_render) {
		auto old_st = ctx.gui;
		ctx.gui = new_st;
		if (ctx.rbuf.Advance(id, checkbox_render(id, cb, ctx))) {
			*val = st.value;
		} else {
			ctx.gui = old_st;
		}
	} else {
		ctx.rbuf.Advance(id);
	}

	return st.event == MouseEvent::DOWN;
}

bool radio_button(ID_TYPE id, const char* label, bool val, float x, float y, Context& ctx, bool force_draw)
{
	egui::RadioButton rbtn({ { x, y, val, label, Callback::GetLabelSize(label) } });
	auto st = radio_button_update(id, rbtn, ctx);

	auto new_st = calc_gui_state(st.event, ctx.gui, id);
	bool need_render = force_draw || new_st != ctx.gui || ctx.rbuf.IsRebuilding();

	if (need_render) {
		auto old_st = ctx.gui;
		ctx.gui = new_st;
		if (!ctx.rbuf.Advance(id, radio_button_render(id, rbtn, ctx))) {
			ctx.gui = old_st;
		}
	} else {
		ctx.rbuf.Advance(id);
	}

	return st.event == MouseEvent::DOWN;
}

bool arrow_button(ID_TYPE id, Direction dir, float x, float y, float height, bool repeat, Context& ctx, bool force_draw)
{
	egui::ArrowButton ab({ { x, y, height, dir } });
	auto st = arrow_button_update(id, ab, ctx);

	auto new_st = calc_gui_state(st.event, ctx.gui, id);
	bool need_render = force_draw || new_st != ctx.gui || ctx.rbuf.IsRebuilding();

	if (need_render) {
		auto old_st = ctx.gui;
		ctx.gui = new_st;
		if (!ctx.rbuf.Advance(id, arrow_button_render(id, ab, ctx))) {
			ctx.gui = old_st;
		}
	} else {
		ctx.rbuf.Advance(id);
	}

	bool ret = false;
	if (st.event == MouseEvent::DOWN) {
		ret = true;
	} else if (repeat && st.event == MouseEvent::HOLD) {
		ret = ctx.io.hold_time < 0;
		if (ctx.io.hold_time <= 0) {
			ctx.io.hold_time += ctx.config.key_repeat_delay;
		}
	}
	return ret;
}

}