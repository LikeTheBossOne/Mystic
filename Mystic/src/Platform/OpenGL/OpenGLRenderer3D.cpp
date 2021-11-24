#include "OpenGLRenderer3D.h"

#include <iostream>
#include <ostream>


namespace Mystic
{
	bool OpenGLRenderer3D::CreateWindow(uint32_t width, uint32_t height, std::string title)
	{
		// Setup window
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return false;
		}
		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return false;
		}

		glViewport(0, 0, width, height);

		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glfwSetCursorPosCallback(window, MouseMovedCallback);
		glfwSetScrollCallback(window, ScrollCallback);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glfwSwapInterval(0); // Disable VSync

		_window = window;
		return true;
	}

	void OpenGLRenderer3D::RenderBox(glm::mat4 transform, glm::vec4 color)
	{
	}

	glm::vec2 OpenGLRenderer3D::GetMousePosition()
	{
		return _mousePos;
	}

	bool OpenGLRenderer3D::ShouldCloseWindow()
	{
		return glfwWindowShouldClose(_window);
	}

	void OpenGLRenderer3D::ClearScreen()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	bool OpenGLRenderer3D::GetKeyState(int16_t key)
	{
		return glfwGetKey(_window, key);
	}

	bool OpenGLRenderer3D::GetMouseButtonState(int8_t button)
	{
		return glfwGetMouseButton(_window, button);
	}

	void OpenGLRenderer3D::SwapBuffers()
	{
		glfwSwapBuffers(_window);
	}

	void OpenGLRenderer3D::PollEvents()
	{
		glfwPollEvents();
	}

	void OpenGLRenderer3D::SetWindowShouldClose(bool shouldClose)
	{
		glfwSetWindowShouldClose(_window, shouldClose);
	}
}
