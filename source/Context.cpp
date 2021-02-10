#include "easygui/Context.h"

namespace egui
{

void Context::BeginDraw(const ur::Device& dev)
{
	m_old_gui = gui;

	rbuf.Rewind(dev);
}

void Context::EndDraw(const ur::Device& dev, ur::Context& ctx,
	                  const std::shared_ptr<ur::ShaderProgram>& shader)
{
	rbuf.InitVAO(dev);
	rbuf.Draw(ctx, shader);
	//if (rbuf.NeedRebuild()) {
	//	gui = m_old_gui;
	//}
}

void Context::Update(float dt)
{
	const float ht = io.GetHoldTime();
	if (ht >= 0) {
		io.SetHoldTime(ht - dt);
	}

	if (!rbuf.NeedRebuild())
	{
		io = io.FeedEvent(input_events);
		input_events.clear();
	}
}

tess::Painter Context::NewPainter() const
{
	auto pt = tess::Painter();
	if (m_palette) {
		pt.SetPalette(m_palette);
	}
	return pt;
}

void Context::SetPalette(const std::shared_ptr<tess::Palette>& palette) 
{
	m_palette = palette;

	rbuf.GetPainter()->SetPalette(palette);
}

}