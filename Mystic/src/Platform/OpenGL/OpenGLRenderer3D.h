#pragma once
#include <memory>
#include <unordered_map>
#include <vector>
#include <glm/ext/matrix_float4x4.hpp>
#include "../../Mystic/GFX/Renderer3D.h"

namespace Mystic
{
	struct Mesh;
	class Camera;
	class Shader;
	class RenderJob;
	/**
	 * This is not super performant because you construct a shape or sprite/texture every time.
	 * In the future, this should be optimized to reuse shapes/textures or should just switch to 2D OpenGL
	 */
	class OpenGLRenderer3D : public InstRenderer3D
	{
	public:
		OpenGLRenderer3D();
		~OpenGLRenderer3D() override;

		bool API_CreateWindow(uint32_t width, uint32_t height, std::string title) override;
		void ClearScreen() override;
		bool GetKeyState(int16_t key) const override;
		bool GetMouseButtonState(int8_t button) const override;
		void SwapBuffers() override;
		void PollEvents() override;
		void SetFramerateLimit(uint32_t limit) override;


		void UseShaderProgram() override;
		void SetProjectionMatrix(glm::mat4& projectionMatrix) override;
		void SetViewMatrix(glm::mat4& viewMatrix) override;


		void RenderEnt(std::string& meshKey, glm::mat4& modelMat) override;

		void HandleJobs() override;

		uint32_t GetTextureHandle() const override;

	private:
		Ref<Mesh> CreateMesh(std::string& key);

	private:
		Shader* _shader;
		std::unordered_map<std::string, Ref<Mesh>> _meshes;

		std::vector<Ref<RenderJob>> _jobs;
	};
}
