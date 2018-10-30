#include "easygui/Slider.h"
#include "easygui/GuiState.h"
#include "easygui/RenderStyle.h"
#include "easygui/Utility.h"

#include <tessellation/Painter.h>

namespace
{

const float GRID_WIDTH  = 16;
const float GRID_HEIGHT = 16;

const float HALF_GRID_WIDTH  = GRID_WIDTH * 0.5f;
const float HALF_GRID_HEIGHT = GRID_HEIGHT * 0.5f;

}

namespace egui
{

Slider::State slider_update(ID_TYPE id, const Slider& slide, const GuiState& gui_st)
{
	Slider::State st = slide.state;

	const float x = slide.props.x + HALF_GRID_WIDTH;
	const float y = slide.props.y + HALF_GRID_HEIGHT;
	st.event = calc_mouse_event(gui_st, id, x, y, GRID_WIDTH, slide.props.height);

	if (st.event == MouseEvent::DOWN || st.event == MouseEvent::HOLD)
	{
		float mouse_pos = gui_st.mouse_y - (y + HALF_GRID_HEIGHT);
		mouse_pos = std::min(std::max(mouse_pos, 0.0f), slide.props.height);
		float v = (mouse_pos * slide.props.max) / 255;
		if (v != slide.state.value) {
			st.value = v;
		}
	}

	return st;
}

tess::Painter slider_render(ID_TYPE id, const Slider& slide, const GuiState& gui_st, const RenderStyle& rs)
{
	tess::Painter pt;

	float ypos = ((slide.props.height - GRID_HEIGHT) * slide.state.value) / slide.props.max;

	draw_rect(pt, slide.props.x, slide.props.y, GRID_WIDTH * 2, slide.props.height + GRID_HEIGHT, 0xff777777);

	const float x = slide.props.x + HALF_GRID_WIDTH;
	const float y = slide.props.y + HALF_GRID_HEIGHT + ypos;
	if (gui_st.active_item == id || gui_st.hot_item == id) {
		draw_rect(pt, x, y, GRID_WIDTH, GRID_HEIGHT, 0xffffffff);
	} else {
		draw_rect(pt, x, y, GRID_WIDTH, GRID_HEIGHT, 0xffaaaaaa);
	}

	return pt;
}

}