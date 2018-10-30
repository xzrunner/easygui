#pragma once

#include "easygui/typedef.h"

namespace egui
{

struct Entity
{
	Entity() : id(0) {}

	bool operator == (const Entity& e) const {
		return id == e.id;
	}
	bool operator != (const Entity& e) const {
		return id != e.id;
	}

	bool IsNull() const { return id == 0; }
	void Reset() { id = 0; }

	union
	{
		struct
		{
			ID_TYPE index : ENTITY_INDEX_BIT_COUNT;
			ID_TYPE type  : ENTITY_TYPE_BIT_COUNT;
		};
		ID_TYPE id;
	};

}; // Entity

}