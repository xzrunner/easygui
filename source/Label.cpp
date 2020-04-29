#include "easygui/Label.h"
#include "easygui/Callback.h"
#include "easygui/RenderStyle.h"

#include <tessellation/Painter.h>

namespace egui
{

tess::Painter label_render(ur::Context& ctx, const Label& label, const RenderStyle& rs)
{
	tess::Painter pt;

	auto& pp = label.props;
	Callback::DrawLabel(ctx, label.props.text, { pp.x, pp.y }, pp.angle,
		rs.colors[(int)Color::Text], pt);

	return pt;
}

}