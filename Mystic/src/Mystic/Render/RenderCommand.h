#pragma once

#include "RendererAPI.h"
#include "glm/vec4.hpp"
#include "Mystic/Core/Core.h"

namespace Mystic {
	class VertexArray;

	class MYSTIC_API RenderCommand
	{
	public:
		static void Init()
		{
			s_rendererAPI->Init();
		}

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_rendererAPI->SetViewport(x, y, width, height);
		}

		static void SetClearColor(const glm::vec4& color)
		{
			s_rendererAPI->SetClearColor(color);
		}

		static void Clear()
		{
			s_rendererAPI->Clear();
		}

		static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0)
		{
			s_rendererAPI->DrawIndexed(vertexArray, count);
		}
	private:
		inline static Unique<RendererAPI> s_rendererAPI = RendererAPI::Create();;
	};

}
