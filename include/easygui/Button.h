#pragma once

#include "easygui/typedef.h"

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
	} props;

	struct State
	{
		MouseEvent event = MouseEvent::NONE;
	} state;

	struct Component
	{
		ID_TYPE label;
	} comp;
};

struct GuiState;
struct RenderStyle;

Button::State button_update(ID_TYPE id, const Button& btn, const GuiState& gui_st);
tess::Painter button_render(ID_TYPE id, const Button& btn, const GuiState& gui_st, const RenderStyle& rs);

}