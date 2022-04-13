#include "OpenGLWindow.h"

#include <iostream>
#include <ostream>

namespace Mystic
{

	OpenGLWindow::OpenGLWindow()
	{

	}

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}


	void OpenGLWindow::OpenWindow(int width, int height)
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		// glfw window creation
		_window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
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
	}

	void OpenGLWindow::Clear(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLWindow::SwapBuffer()
	{
		glfwSwapBuffers(_window);
	}

	bool OpenGLWindow::ShouldClose()
	{
		return glfwWindowShouldClose(_window);
	}

	void OpenGLWindow::PollEvents()
	{
		glfwPollEvents();
	}
}
