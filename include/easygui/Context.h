#pragma once

#include "easygui/GuiState.h"
#include "easygui/IOState.h"
#include "easygui/RenderStyle.h"
#include "easygui/UpdateConfig.h"
#include "easygui/RenderBuffer.h"

#include <tessellation/Painter.h>

namespace egui
{

struct Context
{
    // render
    ur::Context* ctx = nullptr;

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

	void BeginDraw(const ur::Device& dev);
	void EndDraw(const ur::Device& dev, ur::Context& ctx, 
		const std::shared_ptr<ur::ShaderProgram>& shader);

	void Update(float dt);

	tess::Painter NewPainter() const;

	void SetPalette(const std::shared_ptr<tess::Palette>& palette) {
		m_palette = palette;
	}

private:
	GuiState m_old_gui;

	std::shared_ptr<tess::Palette> m_palette = nullptr;

};

}