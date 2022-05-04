#pragma once

#include "Mystic/GFX/RenderCommand.h"
#include "Mystic/Core/Core.h"

#include "Mystic/GFX/OrthographicCamera.h"
#include "Mystic/GFX/Shader.h"

namespace Mystic {
	class VertexArray;

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static Unique<SceneData> s_SceneData;
	};
}