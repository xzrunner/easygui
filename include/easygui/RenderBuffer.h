#pragma once

#include "easygui/typedef.h"

#include <unirender/noncopyable.h>

#include <memory>
#include <vector>

namespace tess { class Painter; }
namespace ur { class Device; class Context; class VertexArray; class ShaderProgram; }

namespace egui
{

class RenderBuffer : ur::noncopyable
{
public:
	RenderBuffer();

	void Rewind(const ur::Device& dev);
	bool Advance(ID_TYPE id);
	bool Advance(ID_TYPE id, const tess::Painter& pt);

	bool IsRebuilding() const { return m_status == Status::REBUILDING; }
	bool NeedRebuild() const { return m_status == Status::NEED_REBUILD; }

	void InitVAO(const ur::Device& dev);
	void Draw(ur::Context& ctx, const std::shared_ptr<ur::ShaderProgram>& shader) const;

	auto GetPainter() const { return m_pt; }

private:
	void Rebuild();

	void UpdateVertexBuf();
	void UpdateVertexBufCheckSize(const ur::Device& dev);
	void BuildVAO(const ur::Device& dev);

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

    std::shared_ptr<ur::VertexArray> m_va = nullptr;

	bool m_dirty = false;

	bool m_invalid = false;
	mutable int m_last_draw_count = 0;

	size_t m_last_vbo_sz = 0;
	size_t m_last_ebo_sz = 0;

}; // RenderBuffer

}