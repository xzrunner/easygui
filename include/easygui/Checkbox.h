#pragma once

#include "easygui/typedef.h"

namespace tess { class Painter; }

namespace egui
{

struct Checkbox
{
	struct Props
	{
		float x      = 0;
		float y      = 0;
		float width  = 100;
		float height = 100;
		const char* label = nullptr;
	} props;

	struct State
	{
		bool value = false;
		MouseEvent event = MouseEvent::NONE;
	} state;
};

struct GuiState;
struct RenderStyle;

Checkbox::State checkbox_update(ID_TYPE id, const Checkbox& cb, const GuiState& gui_st);
tess::Painter   checkbox_render(ID_TYPE id, const Checkbox& cb, const GuiState& gui_st, const RenderStyle& rs);

}