#pragma once
#include "Mystic/ECS/System.h"
#include "Mystic/Render/Texture.h"

class AsteroidSpawnSystem : public Mystic::System
{
public:
	AsteroidSpawnSystem();
	void OnUpdate(Mystic::Scene* scene, entt::registry& registry, float deltaTime) override;

private:
	const uint8_t ASTEROID_SPAWN_LIMIT = 8;
	Mystic::Ref<Mystic::Texture2D> _asteroidTexture;
};
