#pragma once

#include "easygui/GuiState.h"
#include "easygui/IOState.h"
#include "easygui/RenderStyle.h"
#include "easygui/UpdateConfig.h"
#include "easygui/RenderBuffer.h"

namespace egui
{

struct Context
{
	// state
	GuiState     gui;
	IOState      io;

	// config
	RenderStyle  style;
	UpdateConfig config;

	// render
	RenderBuffer rbuf;
};

}