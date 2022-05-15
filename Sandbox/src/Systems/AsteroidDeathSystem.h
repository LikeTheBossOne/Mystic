#pragma once
#include "Mystic/ECS/System.h"
#include "Mystic/Render/Texture.h"

class AsteroidDeathSystem : public Mystic::System
{
public:
	AsteroidDeathSystem();
	void OnUpdate(Mystic::Scene* scene, entt::registry& registry, float deltaTime) override;

private:
	Mystic::Ref<Mystic::Texture2D> _asteroidTexture;
};
