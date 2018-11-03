#include "easygui/Label.h"
#include "easygui/Callback.h"
#include "easygui/RenderStyle.h"

#include <tessellation/Painter.h>

namespace egui
{

tess::Painter label_render(const Label& label, const RenderStyle& rs)
{
	tess::Painter pt;

	sm::vec2 pos;
	pos.x = label.props.x;
	pos.y = label.props.y;
	Callback::DrawLabel(label.props.text, pos, rs.colors[(int)Color::Text], pt);

	return pt;
}

}