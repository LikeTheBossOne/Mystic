#include "Renderer3D.h"

#include "Shader.h"
#include "../../Platform/OpenGL/OpenGLRenderer3D.h"

namespace Mystic
{
	Shader* Renderer3D::s_boxShader = nullptr;
	InstRenderer3D* Renderer3D::s_renderer = nullptr;

	void Renderer3D::Init()
	{
		s_renderer = new OpenGLRenderer3D();
		//s_boxShader = new Shader("path\\to\\vert", "path\\to\frag");
	}

	void Renderer3D::OpenScene(uint32_t width, uint32_t height, std::string title)
	{
		s_renderer->CreateWindow(width, height, title);
	}

	bool Renderer3D::ShouldCloseWindow()
	{
		return s_renderer->ShouldCloseWindow();
	}

	void Renderer3D::ClearScreen()
	{
		s_renderer->ClearScreen();
	}

	bool Renderer3D::GetKeyState(int16_t key)
	{
		return s_renderer->GetKeyState(key);
	}

	bool Renderer3D::GetMouseButtonState(int8_t button)
	{
		return s_renderer->GetMouseButtonState(button);
	}

	void Renderer3D::SwapBuffers()
	{
		s_renderer->SwapBuffers();
	}

	void Renderer3D::PollEvents()
	{
		s_renderer->PollEvents();
	}

	void Renderer3D::SetWindowShouldClose(bool shouldClose)
	{
		s_renderer->SetWindowShouldClose(shouldClose);
	}

	void Renderer3D::RenderBox(glm::mat4 transform, glm::vec4 color)
	{
		s_boxShader->Use();

		s_boxShader->SetModel(transform);


	}

}
