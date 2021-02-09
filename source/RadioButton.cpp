#include "easygui/RadioButton.h"
#include "easygui/Utility.h"
#include "easygui/Context.h"

namespace
{

sm::vec2 calc_tot_sz(const sm::vec2& label_sz, const egui::RenderStyle& rs)
{
	sm::vec2 ret;
	ret.x = label_sz.x + label_sz.y + rs.frame_padding.x * 2;
	ret.y = label_sz.y + rs.frame_padding.y * 2;
	return ret;
}

}

namespace egui
{

RadioButton::State radio_button_update(ID_TYPE id, const RadioButton& rb, const Context& ctx)
{
	auto sz = calc_tot_sz(rb.props.label_sz, ctx.style);

	RadioButton::State st = rb.state;
	st.event = calc_mouse_event(ctx.gui, ctx.io, id, rb.props.x, rb.props.y, sz.x, sz.y);
	return st;
}

tess::Painter radio_button_render(ID_TYPE id, const RadioButton& rb, const Context& ctx)
{
	auto pt = ctx.NewPainter();

	auto& pp = rb.props;

	auto sz = calc_tot_sz(pp.label_sz, ctx.style);
	sm::vec2 check_sz(sz.y, sz.y);

	const float half_sz = sz.y * 0.5f;
	const sm::vec2 center(pp.x + half_sz, pp.y + half_sz);
	const float radius = half_sz;
	const uint32_t color = ctx.style.colors[(int)get_group3_item_color(id, ctx.gui, Color::FrameBg)];
	pt.AddCircleFilled(center, radius, color);

	if (pp.value)
	{
		const float radius = half_sz - sz.y / 6;
		const uint32_t color = ctx.style.colors[(int)Color::CheckMark];
		pt.AddCircleFilled(center, radius, color);
	}

	if (pp.label)
	{
		const float x = pp.x + ctx.style.frame_padding.x + sz.y;
		const float y = pp.y + ctx.style.frame_padding.y;
		render_text(*ctx.ctx, pt, pp.label, x, y, pp.label_sz.y, ctx.style);
	}

	return pt;
}

}