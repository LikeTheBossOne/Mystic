#pragma once
#include <functional>
#include <unordered_map>

#include "Application.h"
#include "Editor/Editor.h"

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
		Mystic::Editor::Init(1400, 1000, "Editor Window");
		Mystic::Editor::Start();
	}

	const auto app = Mystic::CreateApplication();
	app->Start();
	while (!Mystic::Editor::ShouldClose())
	{
		app->Update();
		app->Render();
		Mystic::Editor::Update();
	}

	delete app;
}

#endif