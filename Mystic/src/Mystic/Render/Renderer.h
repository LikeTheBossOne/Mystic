#pragma once

#include "Mystic/Render/RenderCommand.h"
#include "Mystic/Core/Core.h"

#include "Mystic/Render/OrthographicCamera.h"
#include "Mystic/Render/Shader.h"

namespace Mystic {
	class VertexArray;

	class MYSTIC_API Renderer
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