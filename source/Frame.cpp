#include "easygui/Frame.h"
#include "easygui/Utility.h"
#include "easygui/Context.h"

#include <tessellation/Painter.h>

namespace egui
{

tess::Painter frame_render(ID_TYPE id, const Frame& frame, const Context& ctx)
{
	auto pt = ctx.NewPainter();

	Color col = get_group3_item_color(id, ctx.gui, Color::FrameBg);
	render_frame(pt, { frame.props.x, frame.props.y }, { frame.props.x + frame.props.w, frame.props.y + frame.props.h }, ctx.style.colors[(int)col], ctx.style);

	return pt;
}

}