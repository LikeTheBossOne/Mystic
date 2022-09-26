#include "GameLayer.h"

#include "singleton.h"
#include "Mystic/Core/ServiceLocator.h"
#include "Mystic/Scene/ProjectScene.h"

namespace Game
{
	GameLayer::GameLayer()
	{
		
	}

	void GameLayer::OnAttach()
	{
		Mystic::ServiceLocator& locator = singleton<Mystic::ServiceLocator>();
		Mystic::Application& app = locator.GetApplication();

		Mystic::Ref<Mystic::ProjectScene> projScene = std::make_shared<Mystic::ProjectScene>();

		// TODO: Load level
		//
		//
		//
		//

		_scene = projScene->CreateRuntimeScene();

		_gameCodeLoader = std::make_shared<Mystic::GameCodeLoader>();
		_gameCodeLoader->Start();
	}

	void GameLayer::OnDetach()
	{

	}

	void GameLayer::OnUpdate(float deltaTime)
	{
		_scene->OnUpdate(deltaTime);
		_gameCodeLoader->Update(deltaTime, _scene);
	}
}
