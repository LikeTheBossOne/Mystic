#pragma once
#include <functional>
#include <unordered_map>

#include "Application.h"
#include "Game.h"
#include "Editor/Editor.h"
#include "GFX/Renderer2D.h"

#ifdef MYST_PLATFORM_WINDOWS

extern Mystic::Application* Mystic::CreateApplication();

int main(int argc, char* argv[])
{
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
		Mystic::Renderer2D::Init();
		Mystic::Renderer2D::OpenScene(800, 600, "Game");

		const auto app = Mystic::Ref<Mystic::Application>(Mystic::CreateApplication());
		Mystic::Ref<Mystic::Game> game = std::make_shared<Mystic::Game>(app);
		game->Start();
	}
}

#endif