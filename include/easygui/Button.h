#pragma once

#include "easygui/typedef.h"

#include <SM_Vector.h>

namespace tess { class Painter; }

namespace egui
{

struct Button
{
	struct Props
	{
		float x      = 0;
		float y      = 0;
		float width  = 100;
		float height = 100;

		const char* label = nullptr;
		sm::vec2    label_sz;

	} props;

	struct State
	{
		MouseEvent event = MouseEvent::NONE;
	} state;
};

struct GuiState;
struct RenderStyle;

Button::State button_update(ID_TYPE id, const Button& btn, const GuiState& gui_st);
tess::Painter button_render(ID_TYPE id, const Button& btn, const GuiState& gui_st, const RenderStyle& rs);

}