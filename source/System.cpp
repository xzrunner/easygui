#include "easygui/System.h"
#include "easygui/CompStorage.h"
#include "easygui/CompType.h"

#include "easygui/Window.h"
#include "easygui/Button.h"
#include "easygui/Slider.h"

namespace
{

egui::CompType get_comp_type(egui::ID_TYPE id)
{
	return static_cast<egui::CompType>((id & egui::ENTITY_TYPE_BIT_MASK) >> egui::ENTITY_INDEX_BIT_COUNT);
}

}

namespace egui
{

void System::Update(ID_TYPE id, const GuiState& gui_st, const RenderStyle& rs, const CompStorage& storage)
{
	auto data = storage.Get(id);
	assert(data);
	switch (get_comp_type(id))
	{
	case CompType::Window:
		window_update(id, *static_cast<const Window*>(data), gui_st, rs, storage);
		break;
	case CompType::Button:
		button_update(id, *static_cast<const Button*>(data), gui_st);
		break;
	case CompType::Slider:
		slider_update(id, *static_cast<const Slider*>(data), gui_st, rs);
		break;
	}
}

tess::Painter System::Render(ID_TYPE id, const GuiState& gui_st, const RenderStyle& rs, const CompStorage& storage)
{
	auto data = storage.Get(id);
	assert(data);
	switch (get_comp_type(id))
	{
	case CompType::Window:
		return window_render(id, *static_cast<const Window*>(data), gui_st, rs, storage);
	case CompType::Button:
		return button_render(id, *static_cast<const Button*>(data), gui_st, rs);
	case CompType::Slider:
		return slider_render(id, *static_cast<const Slider*>(data), gui_st, rs);
	}
	return tess::Painter();
}

}