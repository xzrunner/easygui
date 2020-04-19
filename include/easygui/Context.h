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

	// input
	std::vector<InputEvent> input_events;

	void BeginDraw(const ur2::Device& dev);
	void EndDraw(const ur2::Device& dev, ur2::Context& ctx);

	void Update(float dt);

private:
	GuiState m_old_gui;

};

}