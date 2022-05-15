#include "mystpch.h"
#include "GraphicsContext.h"

#include "Mystic/Render/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Mystic {
	Unique<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    assert(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return std::make_unique<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}
		assert(false, "no api found");
		return nullptr;
	}

}