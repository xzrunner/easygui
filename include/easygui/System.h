#pragma once

#include "typedef.h"

#include <tessellation/Painter.h>

namespace egui
{

struct Context;
class CompStorage;

class System
{
public:
	static void Update(ID_TYPE id, const Context& ctx, const CompStorage& storage);
	static tess::Painter Render(ID_TYPE id, const Context& ctx, const CompStorage& storage);

}; // System

}