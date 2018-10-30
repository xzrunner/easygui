#pragma once

#include <functional>

namespace tess { class Painter; }

namespace egui
{

class Callback
{
public:
	struct Funs
	{
		std::function<void(const tess::Painter&)> draw_painter;
	};

	static void RegisterCallback(const Funs& funs);

	//////////////////////////////////////////////////////////////////////////

	static void DrawPainter(const tess::Painter& pt);

}; // Callback

}