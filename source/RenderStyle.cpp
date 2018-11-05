#include "easygui/RenderStyle.h"

#include <cmath>

namespace
{

uint32_t trans_color(float r, float g, float b, float a)
{
	return ((int)std::floor(a * 255) << 24)
		 | ((int)std::floor(b * 255) << 16)
		 | ((int)std::floor(g * 255) << 8)
		 |  (int)std::floor(r * 255);
}

}

namespace egui
{

RenderStyle::RenderStyle()
{
	frame_padding     = sm::vec2(4, 3);
	frame_rounding    = 0;
	frame_border_size = 0;
	grab_min_size     = 10;
	grab_rounding     = 0;

	font_sz = 18;
}

void style_colors_dark(RenderStyle& s)
{
	s.colors[(int)Color::Border]           = trans_color(0.43f, 0.43f, 0.50f, 0.50f);
	s.colors[(int)Color::BorderShadow]     = trans_color(0.00f, 0.00f, 0.00f, 0.00f);

	s.colors[(int)Color::SliderGrab]       = trans_color(0.24f, 0.52f, 0.88f, 1.00f);
	s.colors[(int)Color::SliderGrabActive] = trans_color(0.26f, 0.59f, 0.98f, 1.00f);

	s.colors[(int)Color::CheckMark]        = trans_color(0.26f, 0.59f, 0.98f, 1.00f);

	s.colors[(int)Color::Button]           = trans_color(0.26f, 0.59f, 0.98f, 0.40f);
	s.colors[(int)Color::ButtonHovered]    = trans_color(0.26f, 0.59f, 0.98f, 1.00f);
	s.colors[(int)Color::ButtonActive]     = trans_color(0.06f, 0.53f, 0.98f, 1.00f);

	s.colors[(int)Color::FrameBg]          = trans_color(0.16f, 0.29f, 0.48f, 0.54f);
	s.colors[(int)Color::FrameBgHovered]   = trans_color(0.26f, 0.59f, 0.98f, 0.40f);
	s.colors[(int)Color::FrameBgActive]    = trans_color(0.26f, 0.59f, 0.98f, 0.67f);

	s.colors[(int)Color::Text]             = trans_color(1.00f, 1.00f, 1.00f, 1.00f);
	s.colors[(int)Color::TextDisabled]     = trans_color(0.50f, 0.50f, 0.50f, 1.00f);
}

}