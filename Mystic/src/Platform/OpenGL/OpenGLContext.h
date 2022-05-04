#pragma once

#include "Mystic/GFX/GraphicsContext.h"

struct GLFWwindow;

namespace Mystic {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* _windowHandle;
	};

}