#include <algorithm>
#include <iostream>
#include <Mystic.h>
#include <vector>

#include "Mystic/Editor/Editor.h"
#include <glm/gtx/quaternion.hpp>
#include "Mystic/GFX/Renderer3D.h"
#include "entt/entt.hpp"
#include "Mystic/Scene/ProjectScene.h"
#include "Mystic/ECS/Components/Renderable.h"
#include "Mystic/ECS/Components/Transform.h"
#include "Platform/OpenGL/Mesh.h"

class Sandbox : public Mystic::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{
		
	}

	inline void Start(Mystic::Ref<Mystic::Game> game) override
	{
		// DO NOT REMOVE //
		Application::Start(game);
		///////////////////

		/*Mystic::Ref<Mystic::ProjectScene> scene = game->GetScene();

		Mystic::Ref<entt::registry> reg = scene->GetRegistry();

		const std::string key = "triangle";

		for (int i = 0; i < 10; i++)
		{
			const entt::entity ent = reg->create();
			Mystic::Transform transform =
			{
				glm::vec3((std::rand() % 20) - 10.f, (std::rand() % 20) - 10.f, (std::rand() % 20) - 30.f),
				glm::quat(glm::vec3((std::rand() % 180) - 90.f, (std::rand() % 360), (std::rand() % 360))),
				glm::vec3((std::rand() % 5), (std::rand() % 5), (std::rand() % 5))
			};
			reg->emplace<Mystic::Transform>(ent, transform);
			Mystic::Renderable renderable = { key };
			reg->emplace<Mystic::Renderable>(ent, renderable);
		}*/
	}

	inline void Update() override
	{
		//std::cout << "UPDATE" << std::endl;
	}

	inline void Render() override
	{
		// /std::cout << "RENDER" << std::endl;
	}
	
};

Mystic::Application* Mystic::CreateApplication()
{
	return new Sandbox();
}