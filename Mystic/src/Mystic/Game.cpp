#include "Game.h"
#include "Application.h"
#include "Layer.h"
#include "Events/RenderEvent.h"
#include "Events/UpdateEvent.h"

namespace Mystic
{

	Game::Game(std::shared_ptr<Application> app)
	{
		_registry = std::make_shared<entt::registry>();
		_app = app;
	}

	void Game::Start()
	{
		_app->Start(std::shared_ptr<Game>(this));
		while (!_app->ShouldClose())
		{
			Update();
		}
	}

	void Game::Update()
	{
		_app->Update();
		_app->Render();
	}

	std::shared_ptr<entt::registry> Game::GetRegistry()
	{
		return _registry;
	}
}

