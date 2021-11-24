#pragma once

#include <glad/glad.h>
#include "GLFW/glfw3.h"

#include "../../Mystic/GFX/Renderer3D.h"

namespace Mystic
{
	inline glm::vec2 g_lastPos = {0, 0};
	inline bool g_firstMouse = true;
	
	inline void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	inline void MouseMovedCallback(GLFWwindow* window, double xPos, double yPos)
	{
		if (g_firstMouse) // initially set to true
		{
			g_lastPos = { xPos, yPos };
			g_firstMouse = false;
		}

		float xOffset = xPos - g_lastPos.x;
		float yOffset = g_lastPos.y - yPos; // Y ranges from bottom to top
		g_lastPos.x = xPos;
		g_lastPos.y = yPos;

		//world->GetCamera()->ProcessMouseMovement(xOffset, yOffset, true);
	}

	inline void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
	{
		//world->GetCamera()->ProcessMouseScroll(yOffset);
	}
	
	class OpenGLRenderer3D : public InstRenderer3D
	{
	public:
		bool CreateWindow(uint32_t width, uint32_t height, std::string title) override;
		
		void RenderBox(glm::mat4 transform, glm::vec4 color) override;

		glm::vec2 GetMousePosition() override;
		bool ShouldCloseWindow() override;
		void ClearScreen() override;
		bool GetKeyState(int16_t key) override;
		bool GetMouseButtonState(int8_t button) override;
		void SwapBuffers() override;
		void PollEvents() override;
		void SetWindowShouldClose(bool shouldClose) override;
		
	private:
		
	private:
		GLFWwindow* _window = nullptr;

		glm::vec2 _mousePos = {0, 0};
	};
}
