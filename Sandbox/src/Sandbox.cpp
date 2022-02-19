#include <algorithm>
#include <iostream>
#include <Mystic.h>
#include <vector>

#include "Mystic/Editor/Editor.h"
#include "Mystic/GFX/Renderer2D.h"
#include "entt/entt.hpp"

struct Position
{
	float x;
	float y;
};

struct Velocity
{
	float dx;
	float dy;
};

class Sandbox : public Mystic::Application
{
public:
	Sandbox()
	{
		_registry = {};
	}

	~Sandbox()
	{
		
	}

	inline void Start(Mystic::Ref<Mystic::Game> game) override
	{
		Application::Start(game);
		for (auto i = 0u; i < 10u; ++i) {
			const entt::entity entity = _registry.create();
			_registry.emplace<Position>(entity, i * 1.f, i * 1.f);
			if (i % 2 == 0) { _registry.emplace<Velocity>(entity, i * .1f, i * .1f); }
		}
	}

	inline void Update() override
	{
		const auto view = _registry.view<Position,const Velocity>();

		for (auto [entity, pos, vel] : view.each())
		{
			pos.x += vel.dx;
			pos.y += vel.dy;
		}
	}

	inline void Render() override
	{
		std::cout << "FRAME" << std::endl;

		const auto view = _registry.view<const Position>();
		for (auto [entity, pos] : view.each())
		{
			std::cout << "E: " << static_cast<uint32_t>(entity) << "\t" << pos.x << "," << pos.y << std::endl;
		}

		Mystic::Renderer2D::RenderTriangle();
	}

private:
	entt::registry _registry;
};

Mystic::Application* Mystic::CreateApplication()
{
	return new Sandbox();
}