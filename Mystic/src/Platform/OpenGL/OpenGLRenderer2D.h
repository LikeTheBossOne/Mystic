#pragma once
#include <memory>
#include <unordered_map>
#include <vector>
#include <glm/ext/matrix_float4x4.hpp>
#include "../../Mystic/GFX/Renderer2D.h"

namespace Mystic
{
	struct Model;
	class Camera;
	class Shader;
	class RenderJob;
	/**
	 * This is not super performant because you construct a shape or sprite/texture every time.
	 * In the future, this should be optimized to reuse shapes/textures or should just switch to 2D OpenGL
	 */
	class OpenGLRenderer2D : public InstRenderer2D
	{
	public:
		OpenGLRenderer2D();
		~OpenGLRenderer2D() override;

		bool CreateWindow(uint32_t width, uint32_t height, std::string title) override;
		void RenderTriangle() override;
		void ClearScreen() override;
		bool GetKeyState(int16_t key) const override;
		bool GetMouseButtonState(int8_t button) const override;
		void SwapBuffers() override;
		void PollEvents() override;
		void SetFramerateLimit(uint32_t limit) override;

		void HandleJobs() override;

		uint32_t GetTextureHandle() const override;

	private:
		uint32_t _shaderProgram;
		std::unordered_map<std::string, Ref<Model>> _models;

		std::vector<RenderJob> _jobs;
	};
}
