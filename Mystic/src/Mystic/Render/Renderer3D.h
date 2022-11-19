#pragma once
#include "EditorCamera.h"
#include <Mystic/Render/PerspectiveCamera.h>

namespace Mystic
{
	class Shader;
	class Texture;
	class BufferLayout;
	class Texture2D;
	class Material;
	class Mesh;

	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void RegisterBatch(std::string shaderName, std::string shaderPath, std::string meshName, std::string meshPath, BufferLayout bufferLayout);
		static void RegisterBatch(Ref<Shader> shader, Ref<Mesh> mesh);
		static bool BatchExists(const std::string& shaderName, const std::string& meshName);

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void BeginScene(const PerspectiveCamera& camera); // TODO: Remove
		static void EndScene();
		static void Flush();
		
		static void DrawModel(const glm::mat4& transform, const std::string shaderName, const std::string meshName, int entityID);
		static void DrawModel(const glm::mat4& transform, const std::string shaderName, const std::string meshName, const Ref<Texture2D>&, int entityID);

		// Stats
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t MeshCount = 0;

			uint32_t GetTotalVertexCount() const { return 0; }
		};
		static void ResetStats();
		static Statistics GetStats();

	private:
		static void StartBatches();
	};
}
