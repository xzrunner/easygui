#pragma once

#include "easygui/typedef.h"

#include <boost/noncopyable.hpp>

#include <memory>
#include <vector>

namespace tess { class Painter; }

namespace egui
{

class RenderBuffer : boost::noncopyable
{
public:
	RenderBuffer();
	~RenderBuffer();

	void Rewind();
	bool Advance(ID_TYPE id);
	bool Advance(ID_TYPE id, const tess::Painter& pt);

	bool IsRebuilding() const { return m_status == Status::REBUILDING; }
	bool NeedRebuild() const { return m_status == Status::NEED_REBUILD; }

	void InitVAO();
	void Draw() const;

private:
	void Rebuild();

	void UpdateVertexBuf();
	void UpdateVertexBufCheckSize();
	void BuildVAO();

private:
	enum class Status
	{
		NORMAL,
		NEED_REBUILD,
		REBUILDING,
	};

	struct Item
	{
		ID_TYPE id;
		size_t vert_n, index_n, tex_n;
	};

private:
	std::shared_ptr<tess::Painter> m_pt = nullptr;

	std::vector<Item> m_items;

	size_t m_ptr = 0;
	size_t m_vert_off = 0, m_index_off = 0, m_tex_off = 0;

	Status m_status = Status::NEED_REBUILD;

	unsigned int m_vao = 0;
	unsigned int m_vbo = 0;
	unsigned int m_ebo = 0;

	bool m_dirty = false;

	bool m_invalid = false;
	mutable int m_last_draw_count = 0;

	size_t m_last_vbo_sz = 0;
	size_t m_last_ebo_sz = 0;

}; // RenderBuffer

}