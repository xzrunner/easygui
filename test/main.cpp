//#include <glp_loop.h>

#include <easygui/Utility.h>
#include <easygui/GuiState.h>
#include <easygui/RenderStyle.h>
#include <easygui/RenderBuffer.h>
#include <easygui/ImGui.h>

#include <unirender/Blackboard.h>
#include <unirender/gl/RenderContext.h>
#include <rendergraph/RenderMgr.h>
#include <rendergraph/SpriteRenderer.h>
#include <painting0/Shader.h>
#include <painting2/WindowContext.h>
#include <painting2/Blackboard.h>

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

egui::GuiState      state;
egui::RenderStyle   style;
egui::RenderBuffer  rb;

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
	auto proj = screen2proj(sx, sy);
	int x = static_cast<int>(proj.x);
	int y = static_cast<int>(proj.y);

	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS) {
			egui::feed_event(state, egui::InputEvent(egui::InputType::MOUSE_LEFT_DOWN, x, y));
		} else if (action == GLFW_RELEASE) {
			egui::feed_event(state, egui::InputEvent(egui::InputType::MOUSE_LEFT_UP, x, y));
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_PRESS) {
			egui::feed_event(state, egui::InputEvent(egui::InputType::MOUSE_RIGHT_DOWN, x, y));
		} else if (action == GLFW_RELEASE) {
			egui::feed_event(state, egui::InputEvent(egui::InputType::MOUSE_RIGHT_UP, x, y));
		}
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
}

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	auto proj = screen2proj(xpos, ypos);
	int x = static_cast<int>(proj.x);
	int y = static_cast<int>(proj.y);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ||
		glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		egui::feed_event(state, egui::InputEvent(egui::InputType::MOUSE_DRAG, x, y));
	} else {
		egui::feed_event(state, egui::InputEvent(egui::InputType::MOUSE_MOVE, x, y));
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
	auto ur_rc = std::make_shared<ur::gl::RenderContext>(4096, [&]() {
		rg::RenderMgr::Instance()->Flush();
	});
	ur::Blackboard::Instance()->SetRenderContext(ur_rc);

	auto wc = std::make_shared<pt2::WindowContext>(WIDTH, HEIGHT, 0, 0);
	pt2::Blackboard::Instance()->SetWindowContext(wc);
	rg::RenderMgr::Instance()->SetRenderer(rg::RenderType::SPRITE);
	wc->Bind();

	auto sr = std::static_pointer_cast<rg::SpriteRenderer>(rg::RenderMgr::Instance()->SetRenderer(rg::RenderType::SPRITE));
	auto shader = sr->GetShader();
	shader->Use();
	shader->SetMat4("u_model", sm::mat4().x);
	ur_rc->BindTexture(sr->GetPaletteTexID(), 0);

	egui::style_colors_dark(style);
}

void draw()
{
	rb.Rewind();

	uint32_t uid = 1;
	if (egui::button(uid++, 200, 300, 100, 50, state, style, rb)) {
		std::cout << "on click one" << '\n';
	}
	if (egui::button(uid++, 50, 300, 100, 50, state, style, rb)) {
		std::cout << "on click two" << '\n';
	}

	static float sval0 = 0;
	if (egui::slider(uid++, &sval0, 100, 40, 255, 255, state, style, rb)) {
		printf("slider 0: %f\n", sval0);
	}

	static float sval1 = 0;
	if (egui::slider(uid++, &sval1, 150, 40, 255, 63, state, style, rb)) {
		printf("slider 1: %f\n", sval1);
	}

	static float sval2 = 0;
	if (egui::slider(uid++, &sval2, 200, 40, 255, 15, state, style, rb)) {
		printf("slider 2: %f\n", sval2);
	}

	rb.InitVAO();
	rb.Draw();
}

}

int main()
{
	init_glfw();
	init_render();

//	glp_loop_init(30);

	while (!glfwWindowShouldClose(WND))
	{
//		glp_loop_update();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

		draw();

		glfwSwapBuffers(WND);

		glfwPollEvents();
	}

	return 0;
}