#pragma once

#include "easygui/typedef.h"

#include <SM_Vector.h>

namespace tess { class Painter; }

namespace egui
{

struct ArrowButton
{
	struct Props
	{
		float x = 0;
		float y = 0;
		float height = 10;

		Direction dir;
	} props;

	struct State
	{
		MouseEvent event = MouseEvent::NONE;
	} state;
};

struct Context;

ArrowButton::State arrow_button_update(ID_TYPE id, const ArrowButton& ab, const Context& ctx);
tess::Painter      arrow_button_render(ID_TYPE id, const ArrowButton& ab, const Context& ctx);

}