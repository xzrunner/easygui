#pragma once

#include "easygui/typedef.h"

#include <SM_Vector.h>

namespace tess { class Painter; }

namespace egui
{

struct RadioButton
{
	struct Props
	{
		float x = 0;
		float y = 0;

		bool value = false;

		const char* label = nullptr;
		sm::vec2    label_sz;

	} props;

	struct State
	{
		MouseEvent event = MouseEvent::NONE;
	} state;
};

struct Context;

RadioButton::State radio_button_update(ID_TYPE id, const RadioButton& rb, const Context& ctx);
tess::Painter      radio_button_render(ID_TYPE id, const RadioButton& rb, const Context& ctx);

}