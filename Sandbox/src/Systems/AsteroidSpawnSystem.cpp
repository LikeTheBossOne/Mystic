#include "AsteroidSpawnSystem.h"

#include "Components/AsteroidComponent.h"
#include "Components/AsteroidSpawner.h"
#include "Mystic/ECS/Components/SpriteRendererComponent.h"
#include "Mystic/ECS/Components/TransformComponent.h"
#include "Mystic/ECS/Components/VelocityComponent.h"
#include "Mystic/Scene/Scene.h"

AsteroidSpawnSystem::AsteroidSpawnSystem()
{
	//_asteroidTexture = Mystic::Texture2D::Create("assets/textures/asteroid.png");
}

void AsteroidSpawnSystem::OnUpdate(Mystic::Scene* scene, entt::registry& registry, float deltaTime)
{
	srand(time(NULL));
	auto view = registry.view<AsteroidSpawner>();
	view.each([this, scene](AsteroidSpawner& asteroidSpawner)
		{
			while (asteroidSpawner.TotalAsteroids < ASTEROID_SPAWN_LIMIT)
			{
				Mystic::Entity newAsteroid = scene->CreateEntity("Asteroid");
				scene->EntityAddComponent<AsteroidComponent>(newAsteroid);
				
				Mystic::SpriteRendererComponent& sprite = scene->EntityAddComponent<Mystic::SpriteRendererComponent>(newAsteroid);
				sprite.Texture = _asteroidTexture;

				Mystic::TransformComponent& transform = scene->EntityGetComponent<Mystic::TransformComponent>(newAsteroid);
				transform.Position.x = (std::rand() % 10) - 5;
				transform.Position.y = (std::rand() % 10) - 5;
				float rotationRads = glm::radians((std::rand() % 360) * 1.f);
				transform.Rotation = glm::quat(glm::vec3(0, 0, rotationRads));

				glm::vec3 vel;
				vel.x = cos(rotationRads);
				vel.y = sin(rotationRads);
				vel.z = 0;

				scene->EntityAddComponent<Mystic::VelocityComponent>(newAsteroid, vel);

				asteroidSpawner.TotalAsteroids++;
			}
		});
}
