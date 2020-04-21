#pragma once

#include "easygui/InputEvent.h"
#include "easygui/typedef.h"
#include "easygui/RenderStyle.h"

#include <SM_Vector.h>

namespace ur2 { class Context; }
namespace tess { class Painter; }

namespace egui
{

class GuiState;
class IOState;

bool region_hit(const IOState& state, float x, float y, float w, float h);

void draw_rect(tess::Painter& pt, const sm::vec2& min, const sm::vec2& max, uint32_t color);
void render_frame(tess::Painter& pt, const sm::vec2& min, const sm::vec2& max, uint32_t color, const RenderStyle& rs, bool border = false, float rounding = 0);
void render_text(ur2::Context& ctx, tess::Painter& pt, const char* str, float x, float y, float height, const RenderStyle& rs, bool vert = false);
void render_arrow(tess::Painter& pt, const sm::vec2& min, float height, Direction dir, uint32_t color);

MouseEvent calc_mouse_event(const GuiState& gui_st, const IOState& io_st, ID_TYPE id, float x, float y, float w, float h);
GuiState   calc_gui_state(MouseEvent event, const GuiState& gui_st, ID_TYPE id, bool drag = false);

Color get_group3_item_color(ID_TYPE id, const GuiState& gui_st, Color start);

}