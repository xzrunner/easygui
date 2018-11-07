#include "easygui/Slider.h"
#include "easygui/Utility.h"
#include "easygui/Callback.h"
#include "easygui/Context.h"

#include <tessellation/Painter.h>

#include <sstream>
#include <iomanip>

namespace egui
{

Slider::State slider_update(ID_TYPE id, const Slider& slide, const Context& ctx)
{
	Slider::State st = slide.state;
	if (ctx.gui.IsDragLocked() && ctx.gui.GetActiveItem() != id) {
		return st;
	}

	auto update_val = [](const Slider& slide, const Context& ctx, Slider::State& out)
	{
		auto& pp = slide.props;
		const float grab_sz = ctx.style.grab_min_size;
		float mouse_pos;
		if (!pp.verticle) {
			mouse_pos = ctx.io.GetMouseX() - (pp.x + grab_sz * 0.5f);
		} else {
			mouse_pos = ctx.io.GetMouseY() - (pp.y + grab_sz * 0.5f);
		}
		mouse_pos = std::min(std::max(mouse_pos, 0.0f), pp.length - grab_sz);
		float v = (mouse_pos * pp.max_val) / (pp.length - grab_sz);
		if (v != slide.state.value) {
			out.value = v;
		}
	};

	if (ctx.gui.IsDragLocked() && ctx.gui.GetActiveItem() == id)
	{
		if (ctx.io.IsMouseDown()) {
			update_val(slide, ctx, st);
			st.event = MouseEvent::HOLD;
		} else {
			st.event = MouseEvent::UP;
		}
	}
	else
	{
		auto& pp = slide.props;
		float frame_h, frame_w;
		if (!pp.verticle) {
			frame_h = pp.label_sz.y + ctx.style.frame_padding.y * 2;
			frame_w = pp.length;
		} else {
			frame_h = pp.length;
			frame_w = pp.label_sz.y + ctx.style.frame_padding.x * 2;
		}
		st.event = calc_mouse_event(ctx.gui, ctx.io, id, pp.x, pp.y, frame_w, frame_h);
		if (st.event == MouseEvent::DOWN || st.event == MouseEvent::HOLD) {
			update_val(slide, ctx, st);
		}
	}

	return st;
}

tess::Painter slider_render(ID_TYPE id, const Slider& slide, const Context& ctx)
{
	tess::Painter pt;

	auto& pp = slide.props;

	float frame_w, frame_h;
	if (!pp.verticle) {
		frame_w = pp.length;
		frame_h = pp.label_sz.y + ctx.style.frame_padding.y * 2;
	} else {
		frame_w = pp.label_sz.y + ctx.style.frame_padding.x * 2;
		frame_h = pp.length;
	}

	// frame bg
	sm::vec2 min(pp.x, pp.y);
	sm::vec2 max(pp.x + frame_w, pp.y + frame_h);
	uint32_t color = ctx.style.colors[(int)get_group3_item_color(id, ctx.gui, Color::FrameBg)];
	render_frame(pt, min, max, color, ctx.style);

	// grab
	const float grab_sz = ctx.style.grab_min_size;
	const float grab_pos = ((pp.length - grab_sz) * slide.state.value) / pp.max_val;
	if (!pp.verticle) {
		min.Set(pp.x + grab_pos, pp.y);
		max = min + sm::vec2(grab_sz, frame_h);
	} else {
		min.Set(pp.x, pp.y + grab_pos);
		max = min + sm::vec2(frame_w, grab_sz);
	}
	const Color col = ctx.gui.GetActiveItem() == id ? Color::SliderGrabActive : Color::SliderGrab;
	draw_rect(pt, min, max, ctx.style.colors[(int)col]);

	// value
	std::ostringstream out;
	out << std::setprecision(3) << slide.state.value;
	auto val_str = out.str();
	auto val_sz = Callback::GetLabelSize(val_str.c_str());
	float x, y;
	if (!pp.verticle) {
		x = pp.x + ctx.style.frame_padding.x + frame_w * 0.5f - val_sz.x * 0.5f;
		y = pp.y + ctx.style.frame_padding.y;
	} else {
		x = pp.x + ctx.style.frame_padding.x;
		y = pp.y - ctx.style.frame_padding.x + frame_h * 0.5f;
	}
	render_text(pt, val_str.c_str(), x, y, val_sz.y, ctx.style, pp.verticle);

	// label
	if (pp.label)
	{
		if (!pp.verticle) {
			x = pp.x + ctx.style.frame_padding.x + frame_w;
			y = pp.y + ctx.style.frame_padding.y;
		} else {
			x = pp.x + ctx.style.frame_padding.x;
			y = pp.y - ctx.style.frame_padding.y;
		}
		render_text(pt, pp.label, x, y, pp.label_sz.y, ctx.style, pp.verticle);
	}

	return pt;
}

}