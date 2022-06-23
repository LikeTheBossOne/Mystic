#include "mystpch.h"
#include "Buffer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Mystic {

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		return std::make_shared<OpenGLVertexBuffer>(size);
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		return std::make_shared<OpenGLVertexBuffer>(vertices, size);
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		return std::make_shared<OpenGLIndexBuffer>(indices, size);
	}
}
