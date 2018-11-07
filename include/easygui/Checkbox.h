#pragma once

#include "easygui/typedef.h"

#include <SM_Vector.h>

namespace tess { class Painter; }

namespace egui
{

struct Checkbox
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
		bool       value = false;
		MouseEvent event = MouseEvent::NONE;

		bool NeedRender(const State& next) const {
			return value != next.value;
		}
	} state;
};

struct Context;

Checkbox::State checkbox_update(ID_TYPE id, const Checkbox& cbox, const Context& ctx);
tess::Painter   checkbox_render(ID_TYPE id, const Checkbox& cbox, const Context& ctx);

}