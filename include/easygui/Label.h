#pragma once

namespace ur2 { class Context; }
namespace tess { class Painter; }

namespace egui
{

struct Label
{
	struct Props
	{
		float x = 0;
		float y = 0;
		float angle = 0;
		const char* text = nullptr;
	} props;
};

struct RenderStyle;

tess::Painter label_render(ur2::Context& ctx, const Label& label, const RenderStyle& rs);

}