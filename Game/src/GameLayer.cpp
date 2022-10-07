#include "GameLayer.h"

#include "singleton.h"
#include "Mystic/Core/ServiceLocator.h"
#include "Mystic/Render/RenderCommand.h"
#include "Mystic/Scene/ProjectScene.h"
#include "Mystic/Scene/ProjectSerializer.h"

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

		auto commandLineArgs = locator.GetApplication().GetCommandLineArgs();
		if (commandLineArgs.Count > 1)
		{
			std::string sceneFilePath = commandLineArgs[1];
			Mystic::ProjectSerializer serializer(projScene);
			serializer.DeserializeProject(sceneFilePath);
		}

		projScene->ReloadGameCode();

		_scene = projScene->CreateRuntimeScene();
		//_scene->ReloadGameCode();
		_scene->OnViewportResize(app.GetWindow().GetWidth(),app.GetWindow().GetHeight());
		Mystic::Entity ent = _scene->CreateEntity("NewEnt");
		_scene->EntityAddComponentByName(ent.EntId, "LaserComponent");

		
	}

	void GameLayer::OnDetach()
	{

	}

	void GameLayer::OnUpdate(float deltaTime)
	{
		Mystic::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.3f, 1 });
		Mystic::RenderCommand::Clear();

		_scene->OnUpdate(deltaTime);
		_scene->Render();
	}
}
