#pragma once

#include "easygui/typedef.h"

#include <cstring>

namespace egui
{

struct GuiState
{
	ID_TYPE hot_item    = 0;
	ID_TYPE active_item = 0;
	bool drag_locked    = false;

	bool operator != (const GuiState& st) const {
		return st.hot_item != hot_item
			|| st.active_item != active_item;
	}

}; // GuiState

}