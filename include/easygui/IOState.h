#pragma once

namespace egui
{

struct IOState
{
	int  key_code    = 0;
	bool key_down    = false;

	int  mouse_x     = 0;
	int  mouse_y     = 0;
	int  mouse_dir   = 0;
	bool mouse_left  = false;
	bool mouse_down  = false;

	float hold_time  = 0.0f;
};

}