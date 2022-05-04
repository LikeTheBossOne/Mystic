#include "mystpch.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <cassert>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Mystic {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: _windowHandle(windowHandle)
	{
		assert(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		assert(status, "Failed to initialize Glad!");

		assert(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Mystic requires at least OpenGL version 4.5!");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(_windowHandle);
	}

}