#include "easygui/Button.h"
#include "easygui/Callback.h"
#include "easygui/CompStorage.h"
#include "easygui/Utility.h"
#include "easygui/Context.h"

#include <tessellation/Painter.h>

namespace
{

sm::vec2 calc_tot_sz(const sm::vec2& label_sz, const egui::RenderStyle& rs)
{
	sm::vec2 ret;
	ret.x = label_sz.x + rs.frame_padding.x * 2;
	ret.y = label_sz.y + rs.frame_padding.y * 2;
	return ret;
}

}

namespace egui
{

Button::State button_update(ID_TYPE id, const Button& btn, const Context& ctx)
{
	auto sz = calc_tot_sz(btn.props.label_sz, ctx.style);

	Button::State st = btn.state;
	st.event = calc_mouse_event(ctx.gui, ctx.io, id, btn.props.x, btn.props.y, sz.x, sz.y);
	return st;
}

tess::Painter button_render(ID_TYPE id, const Button& btn, const Context& ctx)
{
	tess::Painter pt;

	auto& pp = btn.props;

	const Color col = get_group3_item_color(id, ctx.gui, Color::Button);
	auto sz = calc_tot_sz(btn.props.label_sz, ctx.style);
	render_frame(pt, { pp.x, pp.y }, { pp.x + sz.x, pp.y + sz.y }, ctx.style.colors[(int)col], ctx.style);

	if (pp.label)
	{
		const float x = pp.x + ctx.style.frame_padding.x;
		const float y = pp.y + ctx.style.frame_padding.y;
		render_text(pt, pp.label, x, y, pp.label_sz.y, ctx.style);
	}

	return pt;
}

}