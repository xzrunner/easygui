#pragma once

#include "easygui/typedef.h"

#include <SM_Vector.h>

namespace tess { class Painter; }

namespace egui
{

struct Combo
{
	struct Props
	{
		float x = 0;
		float y = 0;
		float length = 0;

		const char* label = nullptr;
		sm::vec2    label_sz;

		const char** const items = nullptr;
		int items_count = 0;

	} props;

	struct State
	{
		int  selected_item = -1;
		bool popup = false;

		MouseEvent event = MouseEvent::NONE;
		ID_TYPE    event_id = 0;

		bool NeedRender(const State& next) const {
			return selected_item != next.selected_item
				|| popup != next.popup;
		}

	} state;

}; // Combo

struct Context;

Combo::State  combo_update(ID_TYPE id, const Combo& com, const Context& ctx);
tess::Painter combo_render(ID_TYPE id, const Combo& com, const Context& ctx);

}