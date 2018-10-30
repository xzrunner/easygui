#include "easygui/Callback.h"

namespace
{

egui::Callback::Funs FUNS;

}

namespace egui
{

void Callback::RegisterCallback(const Funs& funs)
{
	FUNS = funs;
}

void Callback::DrawPainter(const tess::Painter& pt)
{
	FUNS.draw_painter(pt);
}

}