#include <algorithm>
#include <iostream>
#include <Mystic.h>
#include "Mystic/Core/EntryPoint.h"
#include <vector>

#include "Mystic/GFX/Renderer.h"
#include "SandboxLayer.h"
#include "Mystic/Scene/ProjectScene.h"

class Sandbox : public Mystic::Application
{
public:
	Sandbox(Mystic::ApplicationCommandLineArgs args) : Application("Sandbox", args)
	{
		PushLayer(new SandboxLayer());
	}

	~Sandbox()
	{
		
	}

	// inline void Start(Mystic::Ref<Mystic::Game> game) override
	//{
		// DO NOT REMOVE //
		//Application::Start(game);
		///////////////////

		/*Mystic::Ref<Mystic::ProjectScene> scene = game->GetScene();

		Mystic::Ref<entt::registry> reg = scene->GetRegistry();

		const std::string key = "triangle";

		for (int i = 0; i < 10; i++)
		{
			const entt::entity ent = reg->create();
			Mystic::TransformComponent transform =
			{
				glm::vec3((std::rand() % 20) - 10.f, (std::rand() % 20) - 10.f, (std::rand() % 20) - 30.f),
				glm::quat(glm::vec3((std::rand() % 180) - 90.f, (std::rand() % 360), (std::rand() % 360))),
				glm::vec3((std::rand() % 5), (std::rand() % 5), (std::rand() % 5))
			};
			reg->emplace<Mystic::TransformComponent>(ent, transform);
			Mystic::Renderable renderable = { key };
			reg->emplace<Mystic::Renderable>(ent, renderable);
		}*/
	// }

	//inline void Update() override
	//{
	//	//std::cout << "UPDATE" << std::endl;
	//}

	//inline void Render() override
	//{
	//	// /std::cout << "RENDER" << std::endl;
	//}
	
};

Mystic::Application* Mystic::CreateApplication(ApplicationCommandLineArgs args)
{
	return new Sandbox(args);
}