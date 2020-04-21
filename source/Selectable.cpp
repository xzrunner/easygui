#include "easygui/Selectable.h"
#include "easygui/Utility.h"
#include "easygui/Context.h"

#include <SM_Rect.h>
#include <tessellation/Painter.h>

namespace
{

sm::vec2 calc_tot_sz(const egui::Selectable::Props& pp, const egui::RenderStyle& style)
{
	sm::vec2 ret;
	ret.x = std::max(pp.length, pp.label_sz.x + style.item_spacing.x);
	ret.y = pp.label_sz.y + get_selectable_vert_padding(style);
	return ret;
}

}

namespace egui
{

Selectable::State selectable_update(ID_TYPE id, const Selectable& sel, const Context& ctx)
{
	Selectable::State st = sel.state;

	auto& pp = sel.props;
	auto sz = calc_tot_sz(pp, ctx.style);
	st.event = calc_mouse_event(ctx.gui, ctx.io, id, pp.x, pp.y, sz.x, sz.y);
	if (st.event == MouseEvent::DOWN) {
		st.value = !st.value;
	}

	return st;
}

tess::Painter selectable_render(ID_TYPE id, const Selectable& sel, const Context& ctx)
{
	tess::Painter pt;

	auto& pp = sel.props;

	const auto sz = calc_tot_sz(pp, ctx.style);

	if (ctx.gui.GetHotItem() == id || ctx.gui.GetActiveItem() == id || sel.state.value)
	{
		const Color col = get_group3_item_color(id, ctx.gui, Color::Header);
		render_frame(pt, { pp.x, pp.y }, { pp.x + sz.x, pp.y + sz.y }, ctx.style.colors[(int)col], ctx.style);
	}

	if (pp.label)
	{
		const float x = pp.x + ctx.style.frame_padding.x;
		const float y = pp.y + ctx.style.frame_padding.y;
		render_text(*ctx.ctx, pt, pp.label, x, y, pp.label_sz.y, ctx.style);
	}

	return pt;
}

float get_selectable_vert_padding(const RenderStyle& style)
{
	return style.item_spacing.y;
}

}