#include "easygui/Context.h"

namespace egui
{

void Context::BeginDraw()
{
	m_old_gui = gui;

	rbuf.Rewind();
}

void Context::EndDraw()
{
	rbuf.InitVAO();
	rbuf.Draw();

	if (rbuf.NeedRebuild()) {
		gui = m_old_gui;
	}
}

void Context::Update(float dt)
{
	const float ht = io.GetHoldTime();
	if (ht >= 0) {
		io.SetHoldTime(ht - dt);
	}

	if (!rbuf.NeedRebuild())
	{
		io.Clear();
		io = io.FeedEvent(input_events);
		input_events.clear();
	}
}

}