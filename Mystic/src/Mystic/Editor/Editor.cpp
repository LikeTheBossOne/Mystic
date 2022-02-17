#include "Editor.h"

#include <iostream>

#include "../../Platform/OpenGL/GLFWEditor.h"
#include "../GFX/Renderer2D.h"

namespace Mystic
{
	std::unique_ptr<InstEditor> Editor::s_editor = nullptr;
	
	void Editor::Init(int windowWidth, int windowHeight, std::string title)
	{
		s_editor = std::make_unique<GLFWEditor>();
		
		s_editor->Init(windowWidth, windowHeight, title);

		Renderer2D::Init();
	}

    

	void Editor::Start()
	{

        
		s_editor->OnStart();

		while (!s_editor->ShouldClose())
		{
			s_editor->PollEvents();
			
			PreRender();

			Render();

			PostRender();
		}
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
