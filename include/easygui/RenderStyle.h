#pragma once

#include <cstdint>

namespace egui
{

enum class Color
{
	Button,
	ButtonHovered,
	ButtonActive,

	Count,

}; // Color

struct RenderStyle
{
	uint32_t colors[(int)Color::Count];

}; // RenderStyle

void style_colors_dark(RenderStyle& style);

}