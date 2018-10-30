#include "easygui/EntityFactory.h"

namespace egui
{

EntityFactory::EntityFactory()
{
	m_next_ids.fill(0);
}

Entity EntityFactory::Create(CompType type)
{
	Entity e;
	e.type  = static_cast<uint32_t>(type);
	e.index = m_next_ids[e.type]++;
	e.index &= ENTITY_INDEX_BIT_MASK;
	return e;
}

}