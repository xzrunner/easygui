#pragma once

#include "easygui/typedef.h"

#include <cstring>

namespace egui
{

struct GuiState
{
	int  key_code    = 0;
	bool key_down    = false;

	int  mouse_x     = 0;
	int  mouse_y     = 0;
	int  mouse_dir   = 0;
	bool mouse_left  = false;
	bool mouse_down  = false;

	ID_TYPE hot_item    = 0;
	ID_TYPE active_item = 0;

	void Begin();
	void End();

	bool operator != (const GuiState& st) const {
		return st.hot_item != hot_item
			|| st.active_item != active_item;
	}

}; // GuiState

}