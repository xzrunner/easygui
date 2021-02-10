#include "easygui/RenderBuffer.h"
#include "easygui/Callback.h"

#include <unirender/DrawState.h>
#include <unirender/Device.h>
#include <unirender/Context.h>
#include <unirender/VertexArray.h>
#include <unirender/IndexBuffer.h>
#include <unirender/VertexBuffer.h>
#include <unirender/VertexInputAttribute.h>
#include <unirender/Factory.h>
#include <tessellation/Painter.h>
#include <tessellation/Palette.h>

#include <assert.h>

namespace egui
{

RenderBuffer::RenderBuffer()
{
	m_pt = std::make_shared<tess::Painter>();
}

void RenderBuffer::Rewind(const ur::Device& dev)
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
		UpdateVertexBufCheckSize(dev);
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
		m_items.push_back({ id, n_vert, n_index, n_tex });
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

void RenderBuffer::InitVAO(const ur::Device& dev)
{
	if (!m_va) {
		BuildVAO(dev);
	}
}

void RenderBuffer::Draw(ur::Context& ctx, const std::shared_ptr<ur::ShaderProgram>& shader) const
{
	if (!m_invalid) {
		m_last_draw_count = m_pt->GetBuffer().indices.size();
	}
	if (m_last_draw_count > 0)
    {
		auto palette = m_pt->GetPalette();
		if (palette) {
			auto tex = palette->GetRelocatedTex();
			if (!tex) {
				tex = palette->GetTexture();
			}
			ctx.SetTexture(0, tex);
		}

        ur::DrawState ds;
		ds.render_state = ur::DefaultRenderState2D();
        ds.vertex_array = m_va;
		ds.program = shader;

        ctx.Draw(ur::PrimitiveType::Triangles, ds, nullptr);
	}
}

void RenderBuffer::Rebuild()
{
	//static int count = 0;
	//printf("rebuild [%d]\n", count++);

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
	tess::Painter pt(*m_pt);
	Callback::RelocateTexcoords(pt);

	auto& buf = pt.GetBuffer();
    if (buf.vertices.empty() || buf.indices.empty()) {
        return;
    }

	auto ibuf_sz = sizeof(unsigned short) * buf.indices.size();
	auto ibuf = m_va->GetIndexBuffer();
	ibuf->Reset(ibuf_sz);
	ibuf->ReadFromMemory(buf.indices.data(), ibuf_sz, 0);
	m_va->SetIndexBuffer(ibuf);

	auto vbuf_sz = sizeof(tess::Painter::Vertex) * buf.vertices.size();
	auto vbuf = m_va->GetVertexBuffer();
	vbuf->Reset(vbuf_sz);
	vbuf->ReadFromMemory(buf.vertices.data(), vbuf_sz, 0);
	m_va->SetVertexBuffer(vbuf);

	//static int count = 0;
	//printf("update vbo, vb size %d, eb size %d [%d]\n", buf.vertices.size(), buf.indices.size(), count++);
}

void RenderBuffer::UpdateVertexBufCheckSize(const ur::Device& dev)
{
	auto& buf = m_pt->GetBuffer();
	if (buf.vertices.size() > m_last_vbo_sz ||
		buf.indices.size() > m_last_ebo_sz) {
        m_va.reset();
		BuildVAO(dev);
	} else {
		UpdateVertexBuf();
	}
}

void RenderBuffer::BuildVAO(const ur::Device& dev)
{
	if (m_pt->IsEmpty()) {
		return;
	}

    m_va = dev.CreateVertexArray();

	tess::Painter pt(*m_pt);
	Callback::RelocateTexcoords(pt);

	auto& buf = pt.GetBuffer();

    auto ibuf_sz = sizeof(uint16_t) * buf.indices.size();
    auto ibuf = dev.CreateIndexBuffer(ur::BufferUsageHint::StaticDraw, ibuf_sz);
    ibuf->ReadFromMemory(buf.indices.data(), ibuf_sz, 0);
    m_va->SetIndexBuffer(ibuf);

    auto vbuf_sz = sizeof(tess::Painter::Vertex) * buf.vertices.size();
    auto vbuf = dev.CreateVertexBuffer(ur::BufferUsageHint::StaticDraw, vbuf_sz);
    vbuf->ReadFromMemory(buf.vertices.data(), vbuf_sz, 0);
    m_va->SetVertexBuffer(vbuf);

    m_va->SetVertexBufferAttrs({
        std::make_shared<ur::VertexInputAttribute>(0, ur::ComponentDataType::Float,        2, 0, 20),	// pos
        std::make_shared<ur::VertexInputAttribute>(1, ur::ComponentDataType::Float,        2, 8, 20),	// uv
		std::make_shared<ur::VertexInputAttribute>(2, ur::ComponentDataType::UnsignedByte, 4, 16, 20)	// col
    });

	m_last_vbo_sz = buf.vertices.size();
	m_last_ebo_sz = buf.indices.size();

	//static int count = 0;
	//printf("create vb size %d, eb size %d [%d]\n",
	//	buf.vertices.size(), buf.indices.size(), count++);
}

}