#pragma once

#include "easygui/CompType.h"
#include "easygui/Entity.h"

#include <boost/noncopyable.hpp>

#include <array>

namespace egui
{

class EntityFactory : boost::noncopyable
{
public:
	EntityFactory();

	Entity Create(CompType type);

private:
	std::array<uint32_t, (int)CompType::Count> m_next_ids;

}; // EntityFactory

}