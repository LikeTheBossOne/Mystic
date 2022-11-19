#include "GameLayer.h"

#include "singleton.h"
#include "Mystic/Core/Application.h"
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
		Mystic::Application& app = Mystic::Application::Get();

		_scene = std::make_shared<Mystic::Scene>();

		auto commandLineArgs = app.GetCommandLineArgs();
		if (commandLineArgs.Count > 1)
		{
			std::string sceneFilePath = commandLineArgs[1];
			Mystic::ProjectSerializer serializer(_scene);
			serializer.DeserializeProject(sceneFilePath);
		}

		_scene->ReloadGameCode();
		
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
		_scene->OnRender();
	}
}
