#include "Renderer3D.h"

#include "../../Platform/OpenGL//OpenGLRenderer3D.h"

namespace Mystic
{
	InstRenderer3D* Renderer3D::s_renderer = nullptr;

	void Renderer3D::Init()
	{
		s_renderer = new OpenGLRenderer3D();
	}

	void Renderer3D::OpenScene(uint32_t width, uint32_t height, std::string title)
	{
		s_renderer->API_CreateWindow(width, height, title);
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

	void Renderer3D::SetFramerateLimit(uint32_t limit)
	{
		s_renderer->SetFramerateLimit(limit);
	}

	uint32_t Renderer3D::GetTextureHandle()
	{
		return s_renderer->GetTextureHandle();
	}

	void Renderer3D::UseShaderProgram()
	{
		s_renderer->UseShaderProgram();
	}

	void Renderer3D::SetProjectionMatrix(glm::mat4& projectionMatrix)
	{
		s_renderer->SetProjectionMatrix(projectionMatrix);
	}

	void Renderer3D::SetViewMatrix(glm::mat4& viewMatrix)
	{
		s_renderer->SetViewMatrix(viewMatrix);
	}

	void Renderer3D::RenderEnt(std::string& meshKey, glm::mat4& modelMat)
	{
		s_renderer->RenderEnt(meshKey, modelMat);
	}

	void Renderer3D::HandleJobs()
	{
		s_renderer->HandleJobs();
	}
}
