#pragma once

#include "easygui/typedef.h"

namespace egui
{

struct GuiState;
struct RenderStyle;
class  RenderBuffer;

bool button(ID_TYPE id, float x, float y, float width, float height,
	        GuiState& gui_st, const RenderStyle& rs, RenderBuffer& rb);

bool slider(ID_TYPE id, float* val, float x, float y, float height, float max,
	        GuiState& gui_st, const RenderStyle& rs, RenderBuffer& rb);

}