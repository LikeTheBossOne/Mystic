#include "AsteroidDeathSystem.h"

#include "Components/AsteroidComponent.h"
#include "Components/LaserComponent.h"
#include "Mystic/ECS/Components/SpriteRendererComponent.h"
#include "Mystic/ECS/Components/TransformComponent.h"
#include "Mystic/ECS/Components/VelocityComponent.h"
#include "Mystic/Scene/Scene.h"

AsteroidDeathSystem::AsteroidDeathSystem()
{
	_asteroidTexture = Mystic::Texture2D::Create("assets/textures/asteroid.png");
}

void AsteroidDeathSystem::OnUpdate(Mystic::Scene* scene, entt::registry& registry, float deltaTime)
{
	auto laserGroup = registry.group<const LaserComponent>(entt::get<Mystic::TransformComponent>);
	auto asteroidGroup = registry.group<const AsteroidComponent>(entt::get<Mystic::TransformComponent>);

	std::vector<entt::entity> asteroidsToBreak;
	std::unordered_set<entt::entity> lasersToDestroy;

	for (auto [astEntity, asteroid, astTrans] : asteroidGroup.each())
	{
		for (auto [laserEntity, laser, laserTrans] : laserGroup.each())
		{
			if (glm::abs(glm::distance(astTrans.Position, laserTrans.Position)) < 1)
			{
				asteroidsToBreak.emplace_back(astEntity);
				lasersToDestroy.emplace(laserEntity);
				break;
			}
		}
	}

	for (auto ast : asteroidsToBreak)
	{
		AsteroidComponent& astComp = registry.get<AsteroidComponent>(ast);
		Mystic::TransformComponent& astTran = registry.get<Mystic::TransformComponent>(ast);
		if (astComp.SplitsLeft > 0)
		{
			for (uint8_t i = 0; i < 2; i++)
			{
				Mystic::Entity newAsteroid = scene->CreateEntity("Asteroid");
				scene->EntityAddComponent<AsteroidComponent>(newAsteroid, astComp.SplitsLeft - 1);

				Mystic::SpriteRendererComponent& sprite = scene->EntityAddComponent<Mystic::SpriteRendererComponent>(newAsteroid);
				sprite.Texture = _asteroidTexture;

				Mystic::TransformComponent& transform = scene->EntityGetComponent<Mystic::TransformComponent>(newAsteroid);
				transform.Position.x = astTran.Position.x;
				transform.Position.y = astTran.Position.y;
				transform.Scale.x = astTran.Scale.x * 0.5f;
				transform.Scale.y = astTran.Scale.y * 0.5f;
				float rotationRads = glm::radians((std::rand() % 360) * 1.f);
				transform.Rotation = glm::quat(glm::vec3(0, 0, rotationRads));

				glm::vec3 vel;
				vel.x = cos(rotationRads);
				vel.y = sin(rotationRads);
				vel.z = 0;

				scene->EntityAddComponent<Mystic::VelocityComponent>(newAsteroid, vel);
			}

			astComp.SplitsLeft--;
		}

		Mystic::Entity astEnt = scene->GetEntity(ast);
		scene->DestroyEntity(astEnt);
	}

	for (auto laser : lasersToDestroy)
	{
		Mystic::Entity laserEnt = scene->GetEntity(laser);
		scene->DestroyEntity(laserEnt);
	}
}
