#pragma once
#include <functional>

#include "Application.h"
#include "Game.h"
#include "ECS/ComponentRegistry.h"
#include "ECS/Components/Renderable.h"
#include "Editor/Editor.h"
//#include "GFX/Renderer3D.h"
//#include "GFX/Window.h"
//#include "Platform/OpenGL/OpenGLWindow.h"
#include "Scene/ProjectScene.h"

#ifdef MYST_PLATFORM_WINDOWS

extern Mystic::Application* Mystic::CreateApplication();

int main(int argc, char* argv[])
{
	// Initialize Component Registry
	// Individual Projects must call AddComponent for all components they add.
	Mystic::ComponentRegistry::AddComponent<Mystic::GUIDComponent>();
	Mystic::ComponentRegistry::AddComponent<Mystic::Transform>();
	Mystic::ComponentRegistry::AddComponent<Mystic::Renderable>();
	


	bool useEditor = false;

	std::vector<std::string> args;
	for (int i = 0; i < argc; i++)
	{
		if (std::string(argv[i]) == "-E")
		{
			useEditor = true;
		}
	}

	if (useEditor)
	{
		std::function<Mystic::Application*()> fn = Mystic::CreateApplication;
		Mystic::Editor::Init(1400, 1000, "Editor Window", fn);
		Mystic::Editor::Start();
		while (!Mystic::Editor::ShouldClose())
		{
			Mystic::Editor::Update();
		}
	}
	else
	{
		//Mystic::Window* window = new Mystic::OpenGLWindow();
		//window->OpenWindow(800, 600);
		//Mystic::Renderer3D::Init();
		//Mystic::Renderer3D::OpenScene(800, 600, "Game");

		//const auto app = Mystic::Ref<Mystic::Application>(Mystic::CreateApplication());
		//Mystic::Ref<Mystic::ProjectScene> projectScene = std::make_shared<Mystic::ProjectScene>();
		//Mystic::Ref<Mystic::RuntimeScene> runtimeScene = projectScene->CreateRuntimeScene();

		//Mystic::Ref<Mystic::Game> game = std::make_shared<Mystic::Game>(app, runtimeScene);
		////game->Start();
		//app->Start(Mystic::Ref<Mystic::Game>(game));
		//while (!app->ShouldClose() && !window->ShouldClose())
		//{
		//	window->Clear(0.1f, 0.1f, 0.1f, 1.f);

		//	game->Update();

		//	Mystic::Renderer3D::HandleJobs();

		//	window->SwapBuffer();
		//	window->PollEvents();
		//}
	}

	return 0;
}

#endif