#pragma once

#include "easygui/typedef.h"

#include <vector>

namespace tess { class Painter; }

namespace egui
{

struct Window
{
	struct Props
	{
		float x = 0;
		float y = 0;
		float width = 100;
		float height = 100;
	} props;

	struct State
	{
		MouseEvent event = MouseEvent::NONE;
	} state;

	struct Component
	{
		std::vector<ID_TYPE> children;
	} comp;
};

struct Context;
class CompStorage;

Window::State window_update(ID_TYPE id, const Window& wnd, const Context& ctx, const CompStorage& storage);
tess::Painter window_render(ID_TYPE id, const Window& wnd, const Context& ctx, const CompStorage& storage);

}