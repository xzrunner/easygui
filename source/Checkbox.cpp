#include "easygui/Checkbox.h"
#include "easygui/Utility.h"
#include "easygui/Context.h"

#include <tessellation/Painter.h>
#include <primitive/Path.h>

namespace
{

sm::vec2 calc_tot_sz(const sm::vec2& label_sz, const egui::RenderStyle& rs)
{
	sm::vec2 ret;
	ret.x = label_sz.x + label_sz.y + rs.frame_padding.x * 2;
	ret.y = label_sz.y + rs.frame_padding.y * 2;
	return ret;
}

void render_check_mark(tess::Painter& pt, const sm::vec2& pos, uint32_t color, float sz)
{
	float edge_width = std::max(sz / 5.0f, 1.0f);
	sz -= edge_width * 0.5f;
	auto p = pos + sm::vec2(edge_width * 0.25f, edge_width * 0.25f);

	float third = sz / 3.0f;
	float bx = p.x + third;
	float by = p.y + third * 0.5f;

	prim::Path path;
	path.MoveTo({ bx - third, by + third });
	path.LineTo({ bx, by });
	path.LineTo({ bx + third * 2, by + third * 2 });
	pt.AddPath(path, color, edge_width);
}

}

namespace egui
{

Checkbox::State checkbox_update(ID_TYPE id, const Checkbox& cbox, const Context& ctx)
{
	Checkbox::State st = cbox.state;

	auto sz = calc_tot_sz(cbox.props.label_sz, ctx.style);
	st.event = calc_mouse_event(ctx.gui, ctx.io, id, cbox.props.x, cbox.props.y, sz.x, sz.y);
	if (st.event == MouseEvent::DOWN) {
		st.value = !st.value;
	}

	return st;
}

tess::Painter checkbox_render(ID_TYPE id, const Checkbox& cbox, const Context& ctx)
{
	tess::Painter pt;

	auto sz = calc_tot_sz(cbox.props.label_sz, ctx.style);

	auto& pp = cbox.props;

	sm::vec2 check_sz(sz.y, sz.y);

	sm::vec2 min(pp.x, pp.y);
	sm::vec2 max(pp.x + sz.y, pp.y + sz.y);
	uint32_t color = ctx.style.colors[(int)get_group3_item_color(id, ctx.gui, Color::FrameBg)];
	render_frame(pt, min, max, color, ctx.style);

	if (cbox.state.value)
	{
		const float check_sz = std::min(sz.x, sz.y);
		const float pad = std::max(1.0f, (float)(int)(check_sz / 6.0f));
		render_check_mark(pt, min + sm::vec2(pad, pad), ctx.style.colors[(int)Color::CheckMark], sz.y - pad * 2.0f);
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