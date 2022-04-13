#include "Renderer2D.h"

#include "../../Platform/OpenGL//OpenGLRenderer2D.h"

namespace Mystic
{
	InstRenderer2D* Renderer2D::s_renderer = nullptr;

	void Renderer2D::Init()
	{
		s_renderer = new OpenGLRenderer2D();
	}

	void Renderer2D::OpenScene(uint32_t width, uint32_t height, std::string title)
	{
		s_renderer->API_CreateWindow(width, height, title);
	}

	void Renderer2D::ClearScreen()
	{
		s_renderer->ClearScreen();
	}

	bool Renderer2D::GetKeyState(int16_t key)
	{
		return s_renderer->GetKeyState(key);
	}

	bool Renderer2D::GetMouseButtonState(int8_t button)
	{
		return s_renderer->GetMouseButtonState(button);
	}

	void Renderer2D::SwapBuffers()
	{
		s_renderer->SwapBuffers();
	}

	void Renderer2D::PollEvents()
	{
		s_renderer->PollEvents();
	}

	void Renderer2D::SetFramerateLimit(uint32_t limit)
	{
		s_renderer->SetFramerateLimit(limit);
	}

	uint32_t Renderer2D::GetTextureHandle()
	{
		return s_renderer->GetTextureHandle();
	}

	void Renderer2D::UseShaderProgram()
	{
		s_renderer->UseShaderProgram();
	}

	void Renderer2D::SetProjectionMatrix(glm::mat4& projectionMatrix)
	{
		s_renderer->SetProjectionMatrix(projectionMatrix);
	}

	void Renderer2D::SetViewMatrix(glm::mat4& viewMatrix)
	{
		s_renderer->SetViewMatrix(viewMatrix);
	}

	void Renderer2D::RenderEnt(std::string& meshKey, glm::mat4& modelMat)
	{
		s_renderer->RenderEnt(meshKey, modelMat);
	}

	void Renderer2D::HandleJobs()
	{
		s_renderer->HandleJobs();
	}
}
