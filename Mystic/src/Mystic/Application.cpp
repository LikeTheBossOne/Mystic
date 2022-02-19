#include "Application.h"

#include "GFX/Renderer2D.h"

namespace Mystic
{
	Application::Application()
	{
	}

	void Application::Start(std::shared_ptr<Mystic::Game> game)
	{
		Game = game;
	}

	void Application::Close()
	{
		_shouldClose = true;
	}

	bool Application::ShouldClose()
	{
		return _shouldClose;
	}
}
