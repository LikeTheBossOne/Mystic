#pragma once
#include <string>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

namespace Mystic
{
	class Shader;
	class Window;

	class InstRenderer3D
	{
	public:
		virtual ~InstRenderer3D() = default;

		virtual bool CreateWindow(uint32_t width, uint32_t height, std::string title) = 0;
		virtual void RenderBox(glm::mat4 transform, glm::vec4 color) = 0;
		virtual bool ShouldCloseWindow() = 0;
		virtual void ClearScreen() = 0;
		virtual bool GetKeyState(int16_t key) = 0;
		virtual bool GetMouseButtonState(int8_t button) = 0;
		virtual void SwapBuffers() = 0;
		virtual void PollEvents() = 0;
		virtual void SetWindowShouldClose(bool shouldClose) = 0;
		
		virtual glm::vec2 GetMousePosition() = 0;
	};

	class Renderer3D
	{
	public:
		static void Init();

		static void OpenScene(uint32_t width, uint32_t height, std::string title);

		static bool ShouldCloseWindow();
		static void ClearScreen();
		static bool GetKeyState(int16_t key);
		static bool GetMouseButtonState(int8_t button);
		static void SwapBuffers();
		static void PollEvents();
		static void SetWindowShouldClose(bool shouldClose);
		
		static void RenderBox(glm::mat4 transform, glm::vec4 color);
	private:
		static glm::mat4 camera;

		static Shader* s_boxShader;

		static InstRenderer3D* s_renderer;
	};

}
