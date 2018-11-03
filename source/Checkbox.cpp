#include "easygui/Checkbox.h"
#include "easygui/Utility.h"
#include "easygui/GuiState.h"
#include "easygui/RenderStyle.h"
#include "easygui/Label.h"

#include <tessellation/Painter.h>
#include <primitive/Path.h>

namespace
{

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

Checkbox::State checkbox_update(ID_TYPE id, const Checkbox& cb, const GuiState& gui_st)
{
	Checkbox::State st = cb.state;
	st.event = calc_mouse_event(gui_st, id, cb.props.x, cb.props.y, cb.props.width, cb.props.height);
	if (st.event == MouseEvent::DOWN) {
		st.value = !st.value;
	}
	return st;
}

tess::Painter checkbox_render(ID_TYPE id, const Checkbox& cb, const GuiState& gui_st, const RenderStyle& rs)
{
	tess::Painter pt;

	auto& pp = cb.props;

	sm::vec2 check_sz(pp.height, pp.height);

	Color col;
	if (gui_st.hot_item == id) {
		if (gui_st.active_item == id) {
			col = Color::FrameBgActive;
		} else {
			col = Color::FrameBgHovered;
		}
	} else {
		col = Color::FrameBg;
	}
	sm::vec2 min(pp.x, pp.y);
	sm::vec2 max(pp.x + pp.height, pp.y + pp.height);
	render_frame(pt, min, max, rs.colors[(int)col], rs);
	if (cb.state.value) {
		const float check_sz = std::min(pp.width, pp.height);
		const float pad = std::max(1.0f, (float)(int)(check_sz / 6.0f));
		render_check_mark(pt, min + sm::vec2(pad, pad), rs.colors[(int)Color::CheckMark], pp.height - pad * 2.0f);
	}

	if (pp.label)
	{
		sm::vec2 pos;
		pos.x = pp.x + rs.frame_padding.x + pp.height;
		pos.y = pp.y + rs.frame_padding.y + pp.height * 0.5f - 2;	// fixme: layout
		Label label({ pos.x, pos.y, pp.label });
		pt.AddPainter(label_render(label, rs));
	}

	return pt;
}

}