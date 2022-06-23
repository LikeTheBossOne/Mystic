#include "SandboxLayer.h"

#include <iostream>

#include "Components/AsteroidComponent.h"
#include "Components/AsteroidSpawner.h"
#include "Components/LaserComponent.h"
#include "Components/PlayerComponent.h"
#include "Mystic/Core/Application.h"
#include "Mystic/ECS/ComponentRegistry.h"
#include "Mystic/ECS/SystemRegistry.h"
#include "Mystic/ECS/Components/CharacterComponent.h"
#include "Mystic/ECS/Components/RigidBodyComponent.h"
#include "Mystic/ECS/Components/SpriteRendererComponent.h"
#include "Mystic/ECS/Components/ThrusterComponent.h"
#include "Mystic/ECS/Components/TransformComponent.h"
#include "Mystic/ECS/Components/VelocityComponent.h"
#include "Mystic/Render/RenderCommand.h"
#include "Mystic/Scene/ProjectScene.h"
#include "Mystic/Scene/ProjectSerializer.h"
#include "Systems/AsteroidDeathSystem.h"
#include "Systems/AsteroidSpawnSystem.h"
#include "Systems/LaserSpawnSystem.h"
#include "Systems/WindowLoopSystem.h"

SandboxLayer::SandboxLayer()
{
	Mystic::ComponentRegistry::AddComponent<AsteroidComponent>();
	Mystic::ComponentRegistry::AddComponent<AsteroidSpawner>();
	Mystic::ComponentRegistry::AddComponent<LaserComponent>();
	Mystic::ComponentRegistry::AddComponent<PlayerComponent>();

	Mystic::SystemRegistry::AddSystem<AsteroidDeathSystem>("AsteroidDeathSystem");
	Mystic::SystemRegistry::AddSystem<AsteroidSpawnSystem>("AsteroidSpawnSystem");
	Mystic::SystemRegistry::AddSystem<LaserSpawnSystem>("LaserSpawnSystem");
	Mystic::SystemRegistry::AddSystem<WindowLoopSystem>("WindowLoopSystem");

	Mystic::SystemRegistry::EnableSystem("VelocitySystem");
	Mystic::SystemRegistry::EnableSystem("AccelerationSystem");
	Mystic::SystemRegistry::EnableSystem("AsteroidDeathSystem");
	Mystic::SystemRegistry::EnableSystem("AsteroidSpawnSystem");
	Mystic::SystemRegistry::EnableSystem("LaserSpawnSystem");
	Mystic::SystemRegistry::EnableSystem("ThrusterMovementSystem");
	Mystic::SystemRegistry::EnableSystem("WindowLoopSystem");

	Mystic::Ref<Mystic::ProjectScene> projScene = std::make_shared<Mystic::ProjectScene>();

	Mystic::ProjectSerializer serializer(projScene);
	serializer.DeserializeProject("assets/scenes/asteroids.myst");

	_scene = projScene->CreateRuntimeScene();
	Mystic::Application& app = Mystic::Application::Get();
	_scene->OnViewportResize(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

	Mystic::Entity playerEnt = _scene->CreateEntity("Player");
	_scene->EntityGetComponent<Mystic::TransformComponent>(playerEnt).Scale *= 0.25;

	_scene->EntityAddComponent<Mystic::VelocityComponent>(playerEnt);
	_scene->EntityAddComponent<Mystic::RigidBodyComponent>(playerEnt);
	_scene->EntityAddComponent<Mystic::CharacterComponent>(playerEnt, true);
	Mystic::Ref<Mystic::Texture2D> shipTex = Mystic::Texture2D::Create("assets/textures/ship.png");
	Mystic::SpriteRendererComponent& sprite = _scene->EntityAddComponent<Mystic::SpriteRendererComponent>(playerEnt);
	sprite.Texture = shipTex;
	_scene->EntityAddComponent<PlayerComponent>(playerEnt);
	_scene->EntityAddComponent<Mystic::ThrusterComponent>(playerEnt, 5.f, 10.f );


	Mystic::Entity astSpawner = _scene->CreateEntity("AsteroidSpawner");
	_scene->EntityAddComponent<AsteroidSpawner>(astSpawner);
}

void SandboxLayer::OnUpdate(float deltaTime)
{
	Mystic::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.3f, 1 });
	Mystic::RenderCommand::Clear();

	std::cout << "Sandbox tick" << std::endl;
	_scene->OnUpdate(deltaTime);
}