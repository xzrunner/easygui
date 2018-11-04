#include "easygui/RenderBuffer.h"
#include "easygui/Callback.h"

#include <tessellation/Painter.h>
#include <unirender/Blackboard.h>
#include <unirender/RenderContext.h>

#include <assert.h>

namespace egui
{

RenderBuffer::RenderBuffer()
{
	m_pt = std::make_shared<tess::Painter>();
}

RenderBuffer::~RenderBuffer()
{
	if (m_vao != 0) {
		auto& rc = ur::Blackboard::Instance()->GetRenderContext();
		rc.ReleaseVAO(m_vao, m_vbo, m_ebo);
	}
}

void RenderBuffer::Rewind()
{
	m_ptr       = 0;
	m_vert_off  = 0;
	m_index_off = 0;
	m_tex_off   = 0;

	switch (m_status)
	{
	case Status::NORMAL:
		if (m_dirty) {
			UpdateVertexBuf();
			m_dirty = false;
		}
		m_invalid = false;
		break;
	case Status::NEED_REBUILD:
		m_dirty   = false;
		m_invalid = true;
		m_status  = Status::REBUILDING;
		break;
	case Status::REBUILDING:
		m_dirty   = false;
		m_invalid = false;
		m_status  = Status::NORMAL;
		UpdateVertexBufCheckSize();
		break;
	}
}

bool RenderBuffer::Advance(ID_TYPE id)
{
	if (m_status == Status::NEED_REBUILD) {
		return false;
	}
	if (m_ptr >= m_items.size()) {
		Rebuild();
		return false;
	}

	auto& item = m_items[m_ptr];
	if (id != item.id) {
		Rebuild();
		return false;
	}
	m_vert_off  += item.vert_n;
	m_index_off += item.index_n;
	m_tex_off   += item.tex_n;

	++m_ptr;

	return true;
}

bool RenderBuffer::Advance(ID_TYPE id, const tess::Painter& pt)
{
	if (m_status == Status::NEED_REBUILD) {
		return false;
	}

	auto& buf = pt.GetBuffer();
	size_t n_vert  = buf.vertices.size();
	size_t n_index = buf.indices.size();
	size_t n_tex   = pt.GetOtherTexRegion().size();

	auto sz = m_items.size();
	if (m_ptr == sz)
	{
		m_pt->AddPainter(pt);
		m_items.push_back({ id, n_vert, n_index });
	}
	else if (m_ptr < sz)
	{
		auto& item = m_items[m_ptr];
		if (item.id != id || item.vert_n != n_vert || item.index_n != n_index || item.tex_n != n_tex) {
			Rebuild();
			return false;
		}
		m_pt->FillPainter(pt, m_vert_off, m_index_off, m_tex_off);
	}
	else
	{
		Rebuild();
		return false;
	}

	m_dirty = true;

	Advance(id);

	return true;
}

void RenderBuffer::InitVAO()
{
	if (m_vao == 0) {
		BuildVAO();
	}
}

void RenderBuffer::Draw() const
{
	if (!m_invalid) {
		m_last_draw_count = m_pt->GetBuffer().indices.size();
	}
	ur::Blackboard::Instance()->GetRenderContext().DrawElementsVAO(
		ur::DRAW_TRIANGLES, 0, m_last_draw_count, m_vao);
}

void RenderBuffer::Rebuild()
{
	printf("rebuild\n");

	m_pt->Clear();

	m_items.clear();

	m_ptr       = 0;
	m_vert_off  = 0;
	m_index_off = 0;
	m_tex_off   = 0;
	m_dirty     = false;
	m_invalid   = true;

	m_status = Status::NEED_REBUILD;
}

void RenderBuffer::UpdateVertexBuf()
{
	auto& rc = ur::Blackboard::Instance()->GetRenderContext();

	tess::Painter pt(*m_pt);
	Callback::RelocateTexcoords(pt);

	auto& buf = pt.GetBuffer();
	rc.UpdateBufferRaw(ur::BUFFER_VERTEX, m_vbo, buf.vertices.data(), buf.vertices.size() * sizeof(tess::Painter::Vertex));
	rc.UpdateBufferRaw(ur::BUFFER_INDEX, m_ebo, buf.indices.data(), buf.indices.size() * sizeof(unsigned short));

	printf("update vbo, vb size %d, eb size, %d\n", buf.vertices.size(), buf.indices.size());
}

void RenderBuffer::UpdateVertexBufCheckSize()
{
	auto& buf = m_pt->GetBuffer();
	if (buf.vertices.size() > m_last_vbo_sz ||
		buf.indices.size() > m_last_ebo_sz) {
		auto& rc = ur::Blackboard::Instance()->GetRenderContext();
		rc.ReleaseVAO(m_vao, m_vbo, m_ebo);
		BuildVAO();
	} else {
		UpdateVertexBuf();
	}
}

void RenderBuffer::BuildVAO()
{
	ur::RenderContext::VertexInfo vi;

	tess::Painter pt(*m_pt);
	Callback::RelocateTexcoords(pt);

	auto& buf = pt.GetBuffer();

	vi.vert_usage = ur::USAGE_DYNAMIC;
	vi.vn         = buf.vertices.size();
	vi.vertices   = buf.vertices.data();
	vi.stride     = sizeof(tess::Painter::Vertex);

	vi.index_usage = ur::USAGE_DYNAMIC;
	vi.in          = buf.indices.size();
	vi.indices     = buf.indices.data();

	vi.va_list.push_back(ur::VertexAttrib("pos", 2, 4, 20, 0));		// pos
	vi.va_list.push_back(ur::VertexAttrib("uv",  2, 4, 20, 8));		// uv
	vi.va_list.push_back(ur::VertexAttrib("col", 4, 1, 20, 16));	// col

	ur::Blackboard::Instance()->GetRenderContext().CreateVAO(vi, m_vao, m_vbo, m_ebo);

	m_last_vbo_sz = vi.vn;
	m_last_ebo_sz = vi.in;

	printf("create vao %d, vb size %d, eb size, %d\n", m_vao, vi.vn, vi.in);
}

}