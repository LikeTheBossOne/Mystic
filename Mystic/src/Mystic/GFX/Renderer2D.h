#pragma once
#include <cstdint>
#include <string>
#include <glm/ext/matrix_float4x4.hpp>

#include "../Core.h"

namespace Mystic
{
	class InstRenderer2D
	{
	public:
		virtual ~InstRenderer2D() = default;

		virtual bool CreateWindow(uint32_t width, uint32_t height, std::string title) = 0;
		virtual void RenderQuad(glm::vec2 location, glm::vec2 size, float rotationDegs, glm::vec4 color) = 0;
		virtual void ClearScreen() = 0;
		virtual bool GetKeyState(int16_t key) const = 0;
		virtual bool GetMouseButtonState(int8_t button) const = 0;
		virtual void SwapBuffers() = 0;
		virtual void PollEvents() = 0;
		virtual void SetFramerateLimit(uint32_t limit) = 0;

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

		// RENDER METHODS //
		static void RenderQuad(glm::vec2 location, glm::vec2 size, float rotationDegs, glm::vec4 color);

	private:
		static InstRenderer2D* s_renderer;
	};
}
