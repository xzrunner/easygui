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

sm::vec2 Callback::GetLabelSize(const char* label)
{
	return FUNS.get_label_sz(label);
}

void Callback::DrawLabel(const char* label, const sm::vec2& pos, float angle, uint32_t color, tess::Painter& pt)
{
	FUNS.draw_label(label, pos, angle, color, pt);
}

void Callback::RelocateTexcoords(tess::Painter& pt)
{
	FUNS.relocate_texcoords(pt);
}

}