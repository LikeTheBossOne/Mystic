#include "Editor.h"

#include <iostream>

#include "../../Platform/OpenGL/GLFWEditor.h"
#include "../GFX/Renderer3D.h"

namespace Mystic
{
	std::unique_ptr<InstEditor> Editor::s_editor = nullptr;
	
	void Editor::Init( int windowWidth, int windowHeight, std::string title, std::function<Application*()> applicationCreater)
	{
		s_editor = std::make_unique<GLFWEditor>();
		
		s_editor->Init(windowWidth, windowHeight, title, applicationCreater);

		Renderer3D::Init();
	}

    

	void Editor::Start()
	{
		s_editor->OnStart();
	}

	bool Editor::ShouldClose()
	{
		return s_editor->ShouldClose();
	}

	void Editor::Update()
	{
		s_editor->PollEvents();

		PreRender();

		Render();

		PostRender();
	}

	void Editor::ShowGameWindow()
	{
		s_editor->ShowGameWindow();
	}

	void Editor::BindFramebuffer()
	{
		s_editor->BindFramebuffer();
	}

	void Editor::UnbindFrameBuffer()
	{
		s_editor->UnbindFramebuffer();
	}

	void Editor::PreRender()
	{
		s_editor->PreRender();
	}


	void Editor::Render()
	{
		s_editor->Render();
	}

	void Editor::PostRender()
	{
		s_editor->PostRender();
	}
}
