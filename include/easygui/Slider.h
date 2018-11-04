#pragma once

#include <SM_Vector.h>

#include "easygui/typedef.h"

namespace tess { class Painter; }

namespace egui
{

struct Slider
{
	struct Props
	{
		float x = 0;
		float y = 0;
		float length = 100;

		float max_val = 1;

		const char* label = nullptr;
		sm::vec2    label_sz;

		bool verticle = false;

	} props;

	struct State
	{
		float      value = 0;
		MouseEvent event = MouseEvent::NONE;
	} state;
};

struct GuiState;
struct RenderStyle;

Slider::State slider_update(ID_TYPE id, const Slider& slide, const GuiState& gui_st, const RenderStyle& rs);
tess::Painter slider_render(ID_TYPE id, const Slider& slide, const GuiState& gui_st, const RenderStyle& rs);

}