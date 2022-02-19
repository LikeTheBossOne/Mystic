#include "Game.h"
#include "Application.h"

namespace Mystic
{

	Game::Game(Ref<Application> app)
	{
		_registry = std::make_shared<entt::registry>();
		_app = app;
	}

	void Game::Start()
	{
		_app->Start(Ref<Game>(this));
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

	Ref<entt::registry> Game::GetRegistry()
	{
		return _registry;
	}
}

