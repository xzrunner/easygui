#include "easygui/Window.h"
#include "easygui/CompStorage.h"
#include "easygui/Utility.h"
#include "easygui/System.h"
#include "easygui/Context.h"

#include <assert.h>

namespace egui
{

Window::State window_update(ID_TYPE id, const Window& wnd, const Context& ctx, const CompStorage& storage)
{
	Window::State st = wnd.state;
//	st.event = calc_mouse_event(gui_st, id, wnd.props.x, wnd.props.y, wnd.props.width, wnd.props.height);

	for (auto& c : wnd.comp.children) {
		System::Update(c, ctx, storage);
	}

	return st;
}

tess::Painter window_render(ID_TYPE id, const Window& wnd, const Context& ctx, const CompStorage& storage)
{
	auto pt = ctx.NewPainter();
	for (auto& c : wnd.comp.children) {
		pt.AddPainter(System::Render(c, ctx, storage));
	}
	return pt;
}

}