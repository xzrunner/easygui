#pragma once

#include <SM_Vector.h>
#include <SM_Rect.h>

#include <functional>

namespace ur { class Context; }
namespace tess { class Painter; }

namespace egui
{

class Callback
{
public:
	struct Funs
	{
		// label
		std::function<sm::vec2(const char* label)> get_label_sz;
		std::function<void(ur::Context& ctx, const char* label, const sm::vec2& pos, float angle, uint32_t color, tess::Painter& pt)> draw_label;

		// dtex
		std::function<void(tess::Painter& pt)> relocate_texcoords;
	};

	static void RegisterCallback(const Funs& funs);

	//////////////////////////////////////////////////////////////////////////

	static sm::vec2 GetLabelSize(const char* label);
	static void DrawLabel(ur::Context& ctx, const char* label, const sm::vec2& pos, float angle, uint32_t color, tess::Painter& pt);

	static void RelocateTexcoords(tess::Painter& pt);

}; // Callback

}