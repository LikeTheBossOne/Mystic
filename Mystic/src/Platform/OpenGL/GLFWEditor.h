#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../../../ImGui/imgui.h"

#include "../../Mystic/Editor/Editor.h"

namespace Mystic
{
	class GLFWEditor : public InstEditor
	{
	public:
		GLFWEditor();
		~GLFWEditor() override;
		
		void Init(int windowWidth, int windowHeigh, std::string windowTitle) override;

		void OnStart() override;
		void OnEnd() override;
		
		void PreRender() override;
		void Render() override;
		void PostRender() override;

		void ShowGameWindow() override;
		void BindFramebuffer() override;
		void UnbindFramebuffer() override;

		bool ShouldClose() override;
		bool PollEvents() override;

	
	private:
		GLFWwindow* _window;
		bool _show_demo_window;
		bool _show_another_window;
		ImVec4 _clear_color;

		uint32_t _FBO;
		uint32_t _texture;

		int32_t _width;
		int32_t _height;
	};
}
