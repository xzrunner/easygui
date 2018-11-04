#include "easygui/Window.h"
#include "easygui/GuiState.h"
#include "easygui/CompStorage.h"
#include "easygui/Utility.h"
#include "easygui/System.h"

#include <tessellation/Painter.h>

#include <assert.h>

namespace egui
{

Window::State window_update(ID_TYPE id, const Window& wnd, const GuiState& gui_st,
	                        const RenderStyle& rs, const CompStorage& storage)
{
	Window::State st = wnd.state;
//	st.event = calc_mouse_event(gui_st, id, wnd.props.x, wnd.props.y, wnd.props.width, wnd.props.height);

	for (auto& c : wnd.comp.children) {
		System::Update(c, gui_st, rs, storage);
	}

	return st;
}

tess::Painter window_render(ID_TYPE id, const Window& wnd, const GuiState& gui_st,
	                        const RenderStyle& rs, const CompStorage& storage)
{
	tess::Painter pt;
	for (auto& c : wnd.comp.children) {
		pt.AddPainter(System::Render(c, gui_st, rs, storage));
	}
	return pt;
}

}