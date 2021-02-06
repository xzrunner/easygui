#pragma once

#include "easygui/CompType.h"
#include "easygui/Entity.h"

#include <unirender/noncopyable.h>

#include <array>

namespace egui
{

class EntityFactory : ur::noncopyable
{
public:
	EntityFactory();

	Entity Create(CompType type);

private:
	std::array<uint32_t, (int)CompType::Count> m_next_ids;

}; // EntityFactory

}