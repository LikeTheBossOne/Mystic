#include "mystpch.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Mystic {

	Unique<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
		case RendererAPI::API::OpenGL:  return std::make_unique<OpenGLRendererAPI>();
		}

		Log::Assert(false, "Unknown RendererAPI!");
		return nullptr;
	}

}