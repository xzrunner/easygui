#pragma once

#include "easygui/typedef.h"

#include <SM_Vector.h>

namespace tess { class Painter; }

namespace egui
{

struct Selectable
{
	struct Props
	{
		float x = 0;
		float y = 0;
		float length = 0;

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

}; // Selectable

struct Context;
struct RenderStyle;

Selectable::State selectable_update(ID_TYPE id, const Selectable& sel, const Context& ctx);
tess::Painter     selectable_render(ID_TYPE id, const Selectable& sel, const Context& ctx);

float get_selectable_vert_padding(const RenderStyle& style);

}