#pragma once

#include "easygui/typedef.h"

namespace egui
{

struct GuiState;
struct RenderStyle;
class  RenderBuffer;

bool button(ID_TYPE id, const char* label, float x, float y, float width, float height,
	        GuiState& gui_st, const RenderStyle& rs, RenderBuffer& rb, bool force_draw = false);

bool slider(ID_TYPE id, const char* label, float* val, float x, float y, float height, float max, bool verticle,
	        GuiState& gui_st, const RenderStyle& rs, RenderBuffer& rb, bool force_draw = false);

void label(ID_TYPE id, const char* text, float x, float y,
	       const RenderStyle& rs, RenderBuffer& rb, bool force_draw = false);

bool checkbox(ID_TYPE id, const char* label, bool* val, float x, float y,
	          GuiState& gui_st, const RenderStyle& rs, RenderBuffer& rb, bool force_draw = false);

}