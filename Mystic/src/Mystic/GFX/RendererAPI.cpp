#include "mystpch.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Mystic {

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	Unique<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
		case RendererAPI::API::OpenGL:  return std::make_unique<OpenGLRendererAPI>();
		}

		assert(false, "Unknown RendererAPI!");
		return nullptr;
	}

}