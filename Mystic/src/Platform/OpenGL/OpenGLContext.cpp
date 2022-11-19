#include "mystpch.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Mystic/Logging/Log.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Mystic {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: _windowHandle(windowHandle)
	{
		Log::Assert(!!windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		Log::Assert(status, "Failed to initialize Glad!");

		Log::Assert(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Mystic requires at least OpenGL version 4.5!");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(_windowHandle);
	}

}