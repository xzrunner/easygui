#pragma once

#include "easygui/typedef.h"

namespace tess { class Painter; }

namespace egui
{

struct Frame
{
	struct Props
	{
		float x = 0;
		float y = 0;
		float w = 0;
		float h = 0;
	} props;
};

struct Context;

tess::Painter frame_render(ID_TYPE id, const Frame& frame, const Context& ctx);

}