#pragma once

#include <SM_Vector.h>

#include <cstdint>

namespace egui
{

enum class Color
{
	Button,
	ButtonHovered,
	ButtonActive,

	Text,
	TextDisabled,

	Count,

}; // Color

struct RenderStyle
{
	RenderStyle();

	uint32_t colors[(int)Color::Count];

	sm::vec2 frame_padding;

	size_t font_sz;

}; // RenderStyle

void style_colors_dark(RenderStyle& style);

}