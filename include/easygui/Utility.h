#pragma once

#include "easygui/InputEvent.h"
#include "easygui/typedef.h"

#include <SM_Vector.h>

namespace tess { class Painter; }

namespace egui
{

struct GuiState;

void feed_event(GuiState& state, InputEvent event/*, const sm::vec2& pos, const sm::vec2& scale*/);

bool region_hit(const GuiState& gui_st, float x, float y, float w, float h);

void draw_rect(tess::Painter& pt, float x, float y, float w, float h, uint32_t color);

MouseEvent calc_mouse_event(const GuiState& gui_st, ID_TYPE id, float x, float y, float w, float h);
GuiState   calc_gui_state(MouseEvent event, const GuiState& gui_st, ID_TYPE id);

}