#pragma once

#include "easygui/typedef.h"

namespace egui
{

struct Context;

void frame(ID_TYPE id, float x, float y, float width, float height, Context& ctx, bool force_draw = false);

bool button(ID_TYPE id, const char* label, float x, float y, float width, float height,
	        Context& ctx, bool force_draw = false);

bool slider(ID_TYPE id, const char* label, float* val, float x, float y, float height, float max, bool verticle,
	        Context& ctx, bool force_draw = false);

void label(ID_TYPE id, const char* text, float x, float y, Context& ctx, bool force_draw = false);

bool selectable(ID_TYPE id, const char* label, bool* val, float x, float y, float length,
	            Context& ctx, bool force_draw = false);

bool checkbox(ID_TYPE id, const char* label, bool* val, float x, float y,
	          Context& ctx, bool force_draw = false);

bool radio_button(ID_TYPE id, const char* label, bool val, float x, float y,
	              Context& ctx, bool force_draw = false);

bool arrow_button(ID_TYPE id, Direction dir, float x, float y, float height, bool repeat,
	              Context& ctx, bool force_draw = false);

bool combo(ID_TYPE id, const char* label, int* curr_item, const char** const items, int items_count,
	       float x, float y, float length, Context& ctx, bool force_draw = false);

}