#pragma once
#include <cstdint>
#include <string>
#include <glm/ext/matrix_float4x4.hpp>

#include "../Core.h"

namespace Mystic
{
	class Camera;
	struct Transform;
	struct Mesh;

	class InstRenderer2D
	{
	public:
		virtual ~InstRenderer2D() = default;

		virtual bool API_CreateWindow(uint32_t width, uint32_t height, std::string title) = 0;
		virtual void ClearScreen() = 0;
		virtual bool GetKeyState(int16_t key) const = 0;
		virtual bool GetMouseButtonState(int8_t button) const = 0;
		virtual void SwapBuffers() = 0;
		virtual void PollEvents() = 0;
		virtual void SetFramerateLimit(uint32_t limit) = 0;


		virtual void UseShaderProgram() = 0;
		virtual void SetProjectionMatrix(glm::mat4& projectionMatrix) = 0;
		virtual void SetViewMatrix(glm::mat4& viewMatrix) = 0;


		virtual void RenderEnt(std::string& meshKey, glm::mat4& modelMat) = 0;

		virtual void HandleJobs() = 0;

		virtual uint32_t GetTextureHandle() const = 0;
	};


	class MYSTIC_API Renderer2D
	{
	public:
		static void Init();

		static void OpenScene(uint32_t width, uint32_t height, std::string title);
		static void ClearScreen();
		static bool GetKeyState(int16_t key);
		static bool GetMouseButtonState(int8_t button);
		static void SwapBuffers();
		static void PollEvents();
		static void SetFramerateLimit(uint32_t limit);
		
		static uint32_t GetTextureHandle();

		// Internal Values //
		//TODO: Consider not setting these explicitly here, because the engine should be able to render to different viewports with different projection matrices
		static void UseShaderProgram();
		static void SetProjectionMatrix(glm::mat4& projectionMatrix);
		static void SetViewMatrix(glm::mat4& viewMatrix);


		// RENDER METHODS //
		static void RenderEnt(std::string& meshKey, glm::mat4& modelMat);

		static void HandleJobs();

	private:
		static InstRenderer2D* s_renderer;
	};
}
