#include "mystpch.h"
#include "UniformBuffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace Mystic {

	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    assert((false, "RendererAPI::None is currently not supported")); return nullptr;
			case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLUniformBuffer>(size, binding);
		}

		assert(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
