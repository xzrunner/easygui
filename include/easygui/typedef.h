#pragma once

#include <cstdint>

namespace egui
{

static const size_t ENTITY_TYPE_BIT_COUNT  = 12;
static const size_t ENTITY_INDEX_BIT_COUNT = 20;
static const size_t ENTITY_TYPE_BIT_MASK   = 0xfff00000;
static const size_t ENTITY_INDEX_BIT_MASK  = 0x000fffff;

using ID_TYPE = std::uint32_t;

enum class MouseEvent
{
	NONE = 0,
	DOWN,
	UP,
	HOLD,
	HOVER,
};

}