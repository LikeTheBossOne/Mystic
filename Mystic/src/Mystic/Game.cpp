#include "Game.h"
#include "Application.h"
#include "Scene.h"

namespace Mystic
{

	Game::Game(Ref<Application> app)
	{
		_app = app;
		_currentScene = std::make_shared<Scene>();
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
		//_app->Render();
		_currentScene->RenderScene();
	}

	Ref<Scene> Game::GetScene()
	{
		return _currentScene;
	}

}

