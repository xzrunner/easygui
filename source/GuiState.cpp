#include "easygui/GuiState.h"

namespace egui
{

void GuiState::Begin()
{
	hot_item = 0;
}

void GuiState::End()
{
	if (!mouse_down) {
		active_item = 0;
	}
}

}