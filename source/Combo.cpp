#include "easygui/Combo.h"
#include "easygui/Context.h"
#include "easygui/Callback.h"
#include "easygui/Utility.h"
#include "easygui/Selectable.h"

#include <tessellation/Painter.h>

namespace
{

int get_curr_item_idx(const egui::Combo& com)
{
	if (com.props.items_count <= 0) {
		return -1;
	}
	int curr = com.state.selected_item;
	if (curr < 0 || curr >= com.props.items_count) {
		curr = 0;
	}
	return curr;
}

float get_header_vert_padding(const egui::RenderStyle& style)
{
	return style.frame_padding.y * 2;
}

//sm::vec2 calc_header_sz(const egui::Combo& com, const egui::RenderStyle& style)
//{
//	sm::vec2 ret;
//
//	//auto idx = get_curr_item_idx(com);
//	//if (idx < 0) {
//	//	return ret;
//	//}
//
//	if (com.props.items_count <= 0) {
//		return ret;
//	}
//	auto idx = 0;
//
//	auto label_sz = egui::Callback::GetLabelSize(com.props.items[idx]);
//	ret.x = std::max(com.props.length, label_sz.x + style.item_inner_spacing.x);
//	ret.y = label_sz.y + get_header_vert_padding(style);
//
//	return ret;
//}

sm::vec2 calc_header_sz(const egui::Combo::Props& pp, const egui::RenderStyle& style)
{
	sm::vec2 ret;
	ret.x = pp.length;
	ret.y = pp.label_sz.y + get_header_vert_padding(style);
	return ret;
}

float header_height_to_label_height(float header, const egui::RenderStyle& style)
{
	return header - get_header_vert_padding(style);
}

float label_height_to_item_height(float label, const egui::RenderStyle& style)
{
	return label + get_selectable_vert_padding(style);
}

}

namespace egui
{

Combo::State combo_update(ID_TYPE id, const Combo& com, const Context& ctx)
{
	Combo::State st = com.state;

	auto& pp = com.props;

	// header
	auto header_sz = calc_header_sz(pp, ctx.style);
	st.event = calc_mouse_event(ctx.gui, ctx.io, id, pp.x, pp.y, header_sz.x, header_sz.y);
	st.event_id = id;
	if (st.event == MouseEvent::DOWN) {
		st.popup = !st.popup;
	}

	// body
	if (st.popup)
	{
		const float label_height = header_height_to_label_height(header_sz.y, ctx.style);
		const float item_height = label_height_to_item_height(label_height, ctx.style);

		float y = pp.y - item_height;
		for (int i = 0; i < pp.items_count; ++i)
		{
			const sm::vec2 label_sz(0, label_height);
			Selectable sel({ { pp.x, y, pp.length, pp.items[i], label_sz },{ i == st.selected_item } });
			const ID_TYPE item_id = id + i + 1;
			auto item_st = selectable_update(item_id, sel, ctx);
			if (item_st.event != MouseEvent::NONE) {
				st.event    = item_st.event;
				st.event_id = item_id;
			}
			if (item_st.value) {
				st.selected_item = i;
			}
			if (item_st.event == MouseEvent::DOWN) {
				st.popup = false;
				st.event = MouseEvent::DOWN;
				st.event_id = id;
				break;
			}
			y -= item_height;
		}
	}

	return st;
}

tess::Painter combo_render(ID_TYPE id, const Combo& com, const Context& ctx)
{
	tess::Painter pt;

	auto& pp = com.props;

	const auto sz = calc_header_sz(pp, ctx.style);

	// header
	Color col = get_group3_item_color(id, ctx.gui, Color::FrameBg);
	render_frame(pt, { pp.x, pp.y }, { pp.x + sz.x, pp.y + sz.y }, ctx.style.colors[(int)col], ctx.style);

	// arrow
	col = get_group3_item_color(id, ctx.gui, Color::Button);
	render_arrow(pt, { pp.x + sz.x - sz.y, pp.y }, sz.y, Direction::DOWN, ctx.style.colors[(int)col]);

	// preview
	auto idx = get_curr_item_idx(com);
	if (idx >= 0 && pp.items[idx])
	{
		const float x = pp.x + ctx.style.item_inner_spacing.x;
		const float y = pp.y/* + ctx.style.frame_padding.y*/;	// fixme: layout
		render_text(pt, pp.items[idx], x, y, sz.y, ctx.style);
	}

	// label
	if (pp.label)
	{
		const float x = pp.x + ctx.style.frame_padding.x + sz.x;
		const float y = pp.y + ctx.style.frame_padding.y;
		render_text(pt, pp.label, x, y, pp.label_sz.y, ctx.style);
	}

	// body
	if (com.state.popup)
	{
		const float label_height = header_height_to_label_height(sz.y, ctx.style);

		// bg
		const float item_height = label_height_to_item_height(label_height, ctx.style);
		const sm::vec2 min(pp.x, pp.y - item_height * pp.items_count);
		const sm::vec2 max(min.x + pp.length, pp.y);
		render_frame(pt, min, max, ctx.style.colors[(int)Color::PopupBg], ctx.style);

		// items
		float y = pp.y - item_height;
		for (int i = 0; i < pp.items_count; ++i)
		{
			const sm::vec2 label_sz(0, label_height);
			Selectable sel({ { pp.x, y, pp.length, pp.items[i], label_sz },{ i == com.state.selected_item } });
			pt.AddPainter(selectable_render(id + i + 1, sel, ctx));
			y -= item_height;
		}
	}

	return pt;
}

}