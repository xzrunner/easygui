#pragma once

#include "typedef.h"

#include <tessellation/Painter.h>

namespace egui
{

struct GuiState;
class CompStorage;
struct RenderStyle;

class System
{
public:
	static void Update(ID_TYPE id, const GuiState& gui_st, const CompStorage& storage);
	static tess::Painter Render(ID_TYPE id, const GuiState& gui_st, const RenderStyle& rs, const CompStorage& storage);

}; // System

}