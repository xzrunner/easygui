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
		float x = 0;
		float y = 0;

		const char* label = nullptr;
		sm::vec2    label_sz;

	} props;

	struct State
	{
		MouseEvent event = MouseEvent::NONE;

		bool NeedRender(const State& next) const {
			return false;
		}
	} state;
};

struct Context;

Button::State button_update(ID_TYPE id, const Button& btn, const Context& ctx);
tess::Painter button_render(ID_TYPE id, const Button& btn, const Context& ctx);

}