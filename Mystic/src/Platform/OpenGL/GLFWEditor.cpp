#include "GLFWEditor.h"

#include "../../../ImGui/imgui.h"
#include "../../../ImGui/imgui_impl_glfw.h"
#include "../../../ImGui/imgui_impl_opengl3.h"
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <cstdio>
#include <iostream>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "../../Mystic/GFX/Renderer2D.h"

namespace Mystic
{
	GLFWEditor::GLFWEditor()
	{
        _window = nullptr;
		_width = 800;
		_height = 600;
	}

	GLFWEditor::~GLFWEditor()
	{
	}
	void GLFWEditor::Init(int windowWidth, int windowHeight, std::string windowTitle)
	{
		_width = windowWidth;
		_height = windowHeight;
	}

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}

	void GLFWEditor::OnStart()
	{
		// glfw: initialize and configure
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		// glfw window creation
		_window = glfwCreateWindow(_width, _height, "LearnOpenGL", NULL, NULL);
		if (_window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(_window);
		glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);

		// glad: load all OpenGL function pointers
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return;
		}


        Renderer2D::OpenScene(1, 1, "");

        _show_demo_window = false;
        _show_another_window = false;
        _clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        return;
	}

	void GLFWEditor::OnEnd()
	{
		// Cleanup
		//ImGui_ImplOpenGL3_Shutdown();
		//ImGui_ImplGlfw_Shutdown();
		//ImGui::DestroyContext();

		glfwDestroyWindow(_window);
		glfwTerminate();
	}

	void GLFWEditor::PreRender()
	{
		//ImGui_ImplOpenGL3_NewFrame();
		//ImGui_ImplGlfw_NewFrame();
		//ImGui::NewFrame();
	}

	void GLFWEditor::Render()
	{
		Mystic::Renderer2D::ClearScreen();
		Mystic::Renderer2D::RenderTriangle();

		glfwSwapBuffers(_window);
	}

	void GLFWEditor::PostRender()
	{
	}

	bool GLFWEditor::ShouldClose()
	{
		return glfwWindowShouldClose(_window);
	}

	bool GLFWEditor::PollEvents()
	{
		glfwPollEvents();
		return true;
	}
}


