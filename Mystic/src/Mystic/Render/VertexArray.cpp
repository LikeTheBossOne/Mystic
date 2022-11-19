#include "mystpch.h"
#include "VertexArray.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Mystic {
	Mystic::Ref<VertexArray> VertexArray::Create()
	{
		return std::make_shared<OpenGLVertexArray>();
	}

}