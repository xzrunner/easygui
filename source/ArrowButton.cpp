#include "easygui/ArrowButton.h"
#include "easygui/Utility.h"
#include "easygui/Context.h"

namespace egui
{

ArrowButton::State arrow_button_update(ID_TYPE id, const ArrowButton& ab, const Context& ctx)
{
	auto& pp = ab.props;

	ArrowButton::State st = ab.state;
	st.event = calc_mouse_event(ctx.gui, ctx.io, id, pp.x, pp.y, pp.height, pp.height);
	return st;
}

tess::Painter arrow_button_render(ID_TYPE id, const ArrowButton& ab, const Context& ctx)
{
	auto pt = ctx.NewPainter();

	auto& pp = ab.props;

	const Color col = get_group3_item_color(id, ctx.gui, Color::Button);
	render_frame(pt, { pp.x, pp.y }, { pp.x + pp.height, pp.y + pp.height }, ctx.style.colors[(int)col], ctx.style);

	render_arrow(pt, { pp.x, pp.y }, pp.height, pp.dir, ctx.style.colors[(int)Color::Text]);

	return pt;
}

}