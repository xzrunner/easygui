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
	frame_padding      = sm::vec2(4, 3);
	frame_rounding     = 0;
	frame_border_size  = 0;
	item_spacing       = sm::vec2(8, 4);
	item_inner_spacing = sm::vec2(4, 4);
	grab_min_size      = 10;
	grab_rounding      = 0;

	font_sz = 18;
}

void style_colors_dark(RenderStyle& s)
{
	s.colors[(int)Color::WindowBg]             = trans_color(0.06f, 0.06f, 0.06f, 0.94f);
	s.colors[(int)Color::ChildBg]              = trans_color(1.00f, 1.00f, 1.00f, 0.00f);
	s.colors[(int)Color::PopupBg]              = trans_color(0.08f, 0.08f, 0.08f, 0.94f);

	s.colors[(int)Color::Border]               = trans_color(0.43f, 0.43f, 0.50f, 0.50f);
	s.colors[(int)Color::BorderShadow]         = trans_color(0.00f, 0.00f, 0.00f, 0.00f);

	s.colors[(int)Color::SliderGrab]           = trans_color(0.24f, 0.52f, 0.88f, 1.00f);
	s.colors[(int)Color::SliderGrabActive]     = trans_color(0.26f, 0.59f, 0.98f, 1.00f);

	s.colors[(int)Color::CheckMark]            = trans_color(0.26f, 0.59f, 0.98f, 1.00f);

	s.colors[(int)Color::Button]               = trans_color(0.26f, 0.59f, 0.98f, 0.40f);
	s.colors[(int)Color::ButtonHovered]        = trans_color(0.26f, 0.59f, 0.98f, 1.00f);
	s.colors[(int)Color::ButtonActive]         = trans_color(0.06f, 0.53f, 0.98f, 1.00f);

	s.colors[(int)Color::Header]               = trans_color(0.26f, 0.59f, 0.98f, 0.31f);
	s.colors[(int)Color::HeaderHovered]        = trans_color(0.26f, 0.59f, 0.98f, 0.80f);
	s.colors[(int)Color::HeaderActive]         = trans_color(0.26f, 0.59f, 0.98f, 1.00f);

	s.colors[(int)Color::FrameBg]              = trans_color(0.16f, 0.29f, 0.48f, 0.54f);
	s.colors[(int)Color::FrameBgHovered]       = trans_color(0.26f, 0.59f, 0.98f, 0.40f);
	s.colors[(int)Color::FrameBgActive]        = trans_color(0.26f, 0.59f, 0.98f, 0.67f);

	s.colors[(int)Color::ScrollbarBg]          = trans_color(0.02f, 0.02f, 0.02f, 0.53f);
	s.colors[(int)Color::ScrollbarGrab]        = trans_color(0.31f, 0.31f, 0.31f, 1.00f);
	s.colors[(int)Color::ScrollbarGrabHovered] = trans_color(0.41f, 0.41f, 0.41f, 1.00f);
	s.colors[(int)Color::ScrollbarGrabActive]  = trans_color(0.51f, 0.51f, 0.51f, 1.00f);

	s.colors[(int)Color::Text]                 = trans_color(1.00f, 1.00f, 1.00f, 1.00f);
	s.colors[(int)Color::TextDisabled]         = trans_color(0.50f, 0.50f, 0.50f, 1.00f);
}

}