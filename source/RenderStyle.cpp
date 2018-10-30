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

void style_colors_dark(RenderStyle& s)
{
	s.colors[(int)Color::Button]        = trans_color(0.26f, 0.59f, 0.98f, 0.40f);
	s.colors[(int)Color::ButtonHovered] = trans_color(0.26f, 0.59f, 0.98f, 1.00f);
	s.colors[(int)Color::ButtonActive]  = trans_color(0.06f, 0.53f, 0.98f, 1.00f);
}

}