#pragma once

#include <SM_Vector.h>

#include <cstdint>

namespace egui
{

enum class Color
{
	Border,
	BorderShadow,

	Button,
	ButtonHovered,
	ButtonActive,

	CheckMark,

	FrameBg,
	FrameBgHovered,
	FrameBgActive,

	Text,
	TextDisabled,

	Count,

}; // Color

struct RenderStyle
{
	RenderStyle();

	uint32_t colors[(int)Color::Count];

	sm::vec2 frame_padding;
	float    frame_rounding;
	float    frame_border_size;

	size_t font_sz;

}; // RenderStyle

void style_colors_dark(RenderStyle& style);

}