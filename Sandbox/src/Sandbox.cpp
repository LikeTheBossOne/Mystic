#include <algorithm>
#include <iostream>
#include <Mystic.h>
#include <vector>

#include "Mystic/Editor/Editor.h"
#include <glm/gtx/quaternion.hpp>
#include "Mystic/GFX/Renderer2D.h"
#include "entt/entt.hpp"
#include "Mystic/Scene.h"
#include "Mystic/Components/Renderable.h"
#include "Mystic/Components/Transform.h"
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

		Mystic::Ref<Mystic::Scene> scene = game->GetScene();

		Mystic::Ref<entt::registry> reg = scene->GetRegistry();
		
		const entt::entity ent = reg->create();
		Mystic::Transform t = { glm::vec3(0.f, 0.f, -8.0f), glm::quat(glm::vec3(0, 0, 0)), glm::vec3(1, 1, 1) };
		reg->emplace<Mystic::Transform>(ent, t);

		const std::string key = "triangle";
		Mystic::Renderable r = { key };
		reg->emplace<Mystic::Renderable>(ent, r);
	}

	inline void Update() override
	{
		std::cout << "UPDATE" << std::endl;
	}

	inline void Render() override
	{
		std::cout << "RENDER" << std::endl;
	}
	
};

Mystic::Application* Mystic::CreateApplication()
{
	return new Sandbox();
}