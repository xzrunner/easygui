#include <easygui/Utility.h>
#include <easygui/ImGui.h>
#include <easygui/Callback.h>
#include <easygui/Context.h>

#include <glp_loop.h>
#include <unirender/Blackboard.h>
#include <unirender/gl/RenderContext.h>
#include <tessellation/Palette.h>
#include <tessellation/Painter.h>
#include <rendergraph/RenderMgr.h>
#include <rendergraph/SpriteRenderer.h>
#include <rendergraph/Callback.h>
#include <painting0/Shader.h>
#include <painting2/WindowContext.h>
#include <painting2/Blackboard.h>
#include <painting2/Textbox.h>
#include <painting2/RenderContext.h>
#include <facade/GTxt.h>
#include <facade/DTex.h>
#include <facade/Facade.h>
#include <facade/LoadingList.h>

#include <gl/glew.h>
#include <glfw3.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

namespace
{

const int WIDTH  = 1024;
const int HEIGHT = 768;

GLFWwindow* WND = nullptr;

egui::Context CTX;

pt2::Textbox TEXTBOX;

enum TexType
{
	PALETTE,
	LABEL,
	TEX_COUNT,
};
struct Texture
{
	int id;
	size_t w, h;
};
std::array<Texture, TEX_COUNT> TEXTURES;

int CURR_TEXID = -1;

sm::vec2 screen2proj(float x, float y)
{
	return sm::vec2(x - WIDTH * 0.5f, HEIGHT * 0.5f - y);
}

void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	double sx, sy;
	glfwGetCursorPos(window, &sx, &sy);
	auto proj = screen2proj(static_cast<float>(sx), static_cast<float>(sy));
	int x = static_cast<int>(proj.x);
	int y = static_cast<int>(proj.y);

	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS) {
			CTX.input_events.emplace_back(egui::InputType::MOUSE_LEFT_DOWN, x, y);
		} else if (action == GLFW_RELEASE) {
			CTX.input_events.emplace_back(egui::InputType::MOUSE_LEFT_UP, x, y);
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_PRESS) {
			CTX.input_events.emplace_back(egui::InputType::MOUSE_RIGHT_DOWN, x, y);
		} else if (action == GLFW_RELEASE) {
			CTX.input_events.emplace_back(egui::InputType::MOUSE_RIGHT_UP, x, y);
		}
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
}

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	auto proj = screen2proj(static_cast<float>(xpos), static_cast<float>(ypos));
	int x = static_cast<int>(proj.x);
	int y = static_cast<int>(proj.y);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ||
		glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		CTX.input_events.emplace_back(egui::InputType::MOUSE_DRAG, x, y);
	} else {
		CTX.input_events.emplace_back(egui::InputType::MOUSE_MOVE, x, y);
	}
}

bool init_glfw()
{
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "runtime", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);

//	glfwSetWindowUserPointer(window, this);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	//// Initialize GLEW to setup the OpenGL Function pointers
	//if (glewInit() != GLEW_OK) {
	//	return -1;
	//}

	WND = window;

//	m_rc = std::make_shared<facade::RenderContext>();

	return true;
}

void init_render()
{
	auto ur_rc = std::make_shared<ur::gl::RenderContext>(4096, [&](ur::RenderContext& ctx) {
		ctx.EnableFlushCB(false);
		rg::RenderMgr::Instance()->Flush();
		ctx.EnableFlushCB(true);
	});
	ur::Blackboard::Instance()->SetRenderContext(ur_rc);

	ur_rc->EnableBlend(true);
	ur_rc->SetBlend(ur::BLEND_SRC_ALPHA, ur::BLEND_ONE_MINUS_SRC_ALPHA);

	pt2::Blackboard::Instance()->SetRenderContext(std::make_shared<pt2::RenderContext>());

	auto wc = std::make_shared<pt2::WindowContext>(static_cast<float>(WIDTH), static_cast<float>(HEIGHT), WIDTH, HEIGHT);
	pt2::Blackboard::Instance()->SetWindowContext(wc);
	auto sr = std::static_pointer_cast<rg::SpriteRenderer>(
		rg::RenderMgr::Instance()->SetRenderer(rg::RenderType::SPRITE)
	);
	wc->Bind();

	auto& palette = sr->GetPalette();
	TEXTURES[TexType::PALETTE].id = palette.GetTexID();
	TEXTURES[TexType::PALETTE].w  = palette.GetTexWidth();
	TEXTURES[TexType::PALETTE].h  = palette.GetTexHeight();

	auto& label = TEXTURES[TexType::LABEL];
	facade::DTex::Instance()->GetGlyphTexInfo(label.id, label.w, label.h);

	CURR_TEXID = palette.GetTexID();

	auto shader = sr->GetShader();
	shader->Use();
	shader->SetMat4("u_model", sm::mat4().x);
//    shader->UpdateModelMat(sm::mat4().x);

	egui::style_colors_dark(CTX.style);

	// rendergraph callback
	rg::Callback::Funs rg_cb;
	rg_cb.query_cached_tex_quad = [](size_t tex_id, const sm::irect& r, int& out_tex_id)->const float* {
		sx::UID uid = sx::ResourceUID::TexQuad(tex_id, r.xmin, r.ymin, r.xmax, r.ymax);
		int block_id;
		return facade::DTex::Instance()->QuerySymbol(uid, out_tex_id, block_id);
	};
	rg_cb.add_cache_symbol = [](size_t tex_id, int tex_w, int tex_h, const sm::irect& r) {
		sx::UID uid = sx::ResourceUID::TexQuad(tex_id, r.xmin, r.ymin, r.xmax, r.ymax);
		facade::LoadingList::Instance()->AddSymbol(uid, tex_id, tex_w, tex_h, r);
	};
	rg::Callback::RegisterCallback(rg_cb);

	// egui callback
	TEXTBOX.width = 300;
	TEXTBOX.font_size = CTX.style.font_sz;
	TEXTBOX.align_vert = pt2::Textbox::VA_CENTER;
	auto filtpath = "assets\\default2.ttf";
	facade::GTxt::Instance()->LoadFonts({ { "default", filtpath } }, { {} });
	egui::Callback::Funs cb;
	cb.get_label_sz = [](const char* label)->sm::vec2 {
		return facade::GTxt::Instance()->CalcLabelSize(label, TEXTBOX);
	};
	cb.draw_label = [](const char* label, const sm::vec2& pos, float angle, uint32_t color, tess::Painter& pt)
	{
		if (!label) {
			return;
		}

		sm::Matrix2D mat;
		mat.Rotate(angle);
		const float len = TEXTBOX.width * 0.5f;
		float dx = len * cos(angle);
		float dy = len * sin(angle);
		mat.Translate(pos.x + dx, pos.y + dy);

		pt0::Color col;
		col.FromABGR(color);

		facade::GTxt::Instance()->Draw(label, TEXTBOX, mat, col, pt0::Color(0, 0, 0), 0, false, &pt, false);
	};
	cb.relocate_texcoords = [](tess::Painter& pt)
	{
		auto relocate_palette = [](const Texture& tex, tess::Painter::Buffer& buf, int begin, int end)
		{
			assert(begin < end);
			sm::irect qr(0, 0, tex.w, tex.h);
			int cached_texid;
			auto cached_texcoords = rg::Callback::QueryCachedTexQuad(tex.id, qr, cached_texid);
			if (cached_texcoords)
			{
				CURR_TEXID = cached_texid;
				float x = cached_texcoords[0];
				float y = cached_texcoords[1];
				float w = cached_texcoords[2] - cached_texcoords[0];
				float h = cached_texcoords[5] - cached_texcoords[1];

				auto v_ptr = &buf.vertices[begin];
				for (size_t i = 0, n = end - begin + 1; i < n; ++i)
				{
					auto& v = *v_ptr++;
					v.uv.x = x + w * v.uv.x;
					v.uv.y = y + h * v.uv.y;
				}
			}
			else
			{
				rg::Callback::AddCacheSymbol(tex.id, tex.w, tex.h, qr);
			}
		};
		auto relocate_label = [](const Texture& tex, tess::Painter::Buffer& buf, int begin, int end)
		{
			assert((end - begin + 1) % 4 == 0);
			for (int i = begin; i < end; i += 4)
			{
				auto min = buf.vertices[i].uv;
				auto max = buf.vertices[i + 2].uv;

				sm::rect r;
				r.xmin = tex.w * min.x;
				r.xmax = tex.w * max.x;
				r.ymin = tex.h * min.y;
				r.ymax = tex.h * max.y;

				sm::irect qr;
				qr.xmin = static_cast<int>(tex.w * min.x);
				qr.xmax = static_cast<int>(tex.w * max.x);
				qr.ymin = static_cast<int>(tex.h * min.y);
				qr.ymax = static_cast<int>(tex.h * max.y);

				int cached_texid;
				auto cached_texcoords = rg::Callback::QueryCachedTexQuad(tex.id, qr, cached_texid);
				if (cached_texcoords)
				{
					CURR_TEXID = cached_texid;
					auto v_ptr = &buf.vertices[i];
					v_ptr[0].uv.x = cached_texcoords[0];
					v_ptr[0].uv.y = cached_texcoords[1];
					v_ptr[1].uv.x = cached_texcoords[2];
					v_ptr[1].uv.y = cached_texcoords[3];
					v_ptr[2].uv.x = cached_texcoords[4];
					v_ptr[2].uv.y = cached_texcoords[5];
					v_ptr[3].uv.x = cached_texcoords[6];
					v_ptr[3].uv.y = cached_texcoords[7];
				}
				else
				{
					rg::Callback::AddCacheSymbol(tex.id, tex.w, tex.h, qr);
				}
			}
		};

		auto& buf = const_cast<tess::Painter::Buffer&>(pt.GetBuffer());
		if (buf.vertices.empty()) {
			return;
		}

		auto& regions = pt.GetOtherTexRegion();
		if (regions.empty())
		{
			relocate_palette(TEXTURES[TexType::PALETTE], buf, 0, buf.vertices.size() - 1);
		}
		else
		{
			auto& palette = TEXTURES[TexType::PALETTE];
			auto& label   = TEXTURES[TexType::LABEL];
			if (regions.front().begin > 0) {
				relocate_palette(palette, buf, 0, regions.front().begin - 1);
			}
			for (int i = 0, n = regions.size(); i < n; ++i)
			{
				auto& r = regions[i];
				if (i > 0) {
					auto& prev = regions[i - 1];
					if (prev.end + 1 < r.begin) {
						relocate_palette(palette, buf, prev.end + 1, r.begin - 1);
					}
				}
				assert(r.texid == label.id);
				relocate_label(label, buf, r.begin, r.end);
			}
			if (regions.back().end < static_cast<int>(buf.vertices.size() - 1)) {
				relocate_palette(palette, buf, regions.back().end + 1, buf.vertices.size() - 1);
			}
		}
	};
	egui::Callback::RegisterCallback(cb);
}

void draw()
{
	static bool last_frame_dirty = false;

	ur::Blackboard::Instance()->GetRenderContext().BindTexture(CURR_TEXID, 0);

	CTX.BeginDraw();

	uint32_t uid = 1;
	if (egui::button(uid++, "btn0", 200, 300, 100, 50, CTX, last_frame_dirty)) {
		std::cout << "on click one" << '\n';
	}
	if (egui::button(uid++, "btn1", 50, 300, 100, 50, CTX, last_frame_dirty)) {
		std::cout << "on click two" << '\n';
	}

	static float sval0 = 0;
	if (egui::slider(uid++, "slider 0", &sval0, 100, -50, 255, 255, false, CTX, last_frame_dirty)) {
		printf("slider 0: %f\n", sval0);
	}

	static float sval1 = 0;
	if (egui::slider(uid++, "slider 1", &sval1, 100, 50, 255, 63, false, CTX, last_frame_dirty)) {
		printf("slider 1: %f\n", sval1);
	}

	static float sval2 = 0;
	if (egui::slider(uid++, "slider 2 zz", &sval2, 50, -100, 255, 15, true, CTX, last_frame_dirty)) {
		printf("slider 2: %f\n", sval2);
	}

	egui::label(uid++, "hello world.", -200, 0, CTX, last_frame_dirty);

	static bool checkbox = false;
	egui::checkbox(uid++, "checkbox", &checkbox, -200, 100, CTX, last_frame_dirty);

	static int radio_button = 0;
	if (egui::radio_button(uid++, "radio button 0", radio_button == 0, -200, 250, CTX, last_frame_dirty)) {
		radio_button = 0;
	}
	if (egui::radio_button(uid++, "radio button 1", radio_button == 1, -200, 220, CTX, last_frame_dirty)) {
		radio_button = 1;
	}
	if (egui::radio_button(uid++, "radio button 2", radio_button == 2, -200, 190, CTX, last_frame_dirty)) {
		radio_button = 2;
	}

	if (egui::arrow_button(uid++, egui::Direction::LEFT, -200, 280, 20, true, CTX, last_frame_dirty)) {
		printf("arrow left\n");
	}
	if (egui::arrow_button(uid++, egui::Direction::RIGHT, -170, 280, 20, false, CTX, last_frame_dirty)) {
		printf("arrow right\n");
	}

	static bool sel0 = true;
	egui::selectable(uid++, "selectable 0", &sel0, -200, 160, 100, CTX, last_frame_dirty);
	static bool sel1 = false;
	egui::selectable(uid++, "selectable 1", &sel1, -200, 140, 100, CTX, last_frame_dirty);

	static int curr_item = 0;
	const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
	egui::combo(uid++, "combo", &curr_item, items, sizeof(items) / sizeof(items[0]), -200, 50, 100, CTX, last_frame_dirty);

	CTX.EndDraw();

	last_frame_dirty = facade::Facade::Instance()->Flush(false);
	rg::RenderMgr::Instance()->Flush();

//	facade::DTex::Instance()->DebugDraw();
}

void update()
{
	static uint32_t last_time = 0;
	uint32_t curr_time = glp_get_time();
	const float dt = (curr_time - last_time) / 1000000.0f;
	last_time = curr_time;

	CTX.Update(dt);
}

}

int main()
{
	init_glfw();
	init_render();

	glp_loop_init(30);

	while (!glfwWindowShouldClose(WND))
	{
		glp_loop_update();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		glClearColor(0, 0, 0, 1.0f);

		draw();
		update();

		glfwSwapBuffers(WND);

		glfwPollEvents();
	}

	return 0;
}