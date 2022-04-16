//#define RYML_SINGLE_HDR_DEFINE_NOW

#include "Game.h"
#include "Application.h"
#include "Scene/RuntimeScene.h"

namespace Mystic
{

	Game::Game(Ref<Application> app, Ref<RuntimeScene> startingScene)
	{
		_app = app;
		_currentScene = startingScene;
		//SceneSerializer serializer(_currentScene);
		//std::string filePath("scenes/scene1.yaml");
		//serializer.DeserializeScene(filePath);
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

	Ref<RuntimeScene> Game::GetScene()
	{
		return _currentScene;
	}

}

