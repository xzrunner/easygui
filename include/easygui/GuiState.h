#pragma once

#include "easygui/typedef.h"

#include <cstring>

#include <stdio.h>

namespace egui
{

class GuiState
{
public:
	GuiState() = default;
	GuiState(const GuiState& st)
		: m_drag_locked(st.m_drag_locked)
	{
		SetHotItem(st.m_hot_item);
		SetActiveItem(st.m_active_item);
		SetPopupItem(st.m_popup_item);
	}
	GuiState& operator = (const GuiState& st)
	{
		SetHotItem(st.m_hot_item);
		SetActiveItem(st.m_active_item);
		SetPopupItem(st.m_popup_item);
		m_drag_locked = st.m_drag_locked;
		return *this;
	}

	bool NeedRender(const GuiState& next, egui::ID_TYPE id) const {
		return m_hot_item    != next.m_hot_item
			|| m_active_item != next.m_active_item
			|| m_popup_item  != next.m_popup_item
			|| m_last_hot_item    == id
			|| m_last_active_item == id
			|| m_last_popup_item  == id;
	}

	void SetHotItem(ID_TYPE id) {
		SetWithLast(m_hot_item, m_last_hot_item, id);
	}
	void SetActiveItem(ID_TYPE id) {
		SetWithLast(m_active_item, m_last_active_item, id);
		if (id != m_popup_item && m_popup_item != 0) {
			SetPopupItem(0);
		}
	}
	void SetPopupItem(ID_TYPE id) {
		SetWithLast(m_popup_item, m_last_popup_item, id);
		if (id != 0) {
			SetActiveItem(id);
		}
	}

	ID_TYPE GetHotItem() const    { return m_hot_item; }
	ID_TYPE GetActiveItem() const { return m_active_item; }
	ID_TYPE GetPopupItem() const  { return m_popup_item; }

	void SetDragLocked(bool locked) { m_drag_locked = locked; }
	bool IsDragLocked() const { return m_drag_locked; }

private:
	static void SetWithLast(ID_TYPE& curr, ID_TYPE& last, ID_TYPE val) {
		if (curr != val) {
			last = curr;
		} else {
			last = 0;
		}
		curr = val;
	}

private:
	ID_TYPE m_hot_item    = 0;
	ID_TYPE m_active_item = 0;
	ID_TYPE m_popup_item  = 0;

	bool    m_drag_locked = false;

	ID_TYPE m_last_hot_item    = 0;
	ID_TYPE m_last_active_item = 0;
	ID_TYPE m_last_popup_item  = 0;

}; // GuiState

}