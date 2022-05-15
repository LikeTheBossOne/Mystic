#pragma once
#include "Mystic/ECS/System.h"

class LaserSpawnSystem : public Mystic::System
{
	void OnUpdate(Mystic::Scene* scene, entt::registry& registry, float deltaTime) override;
};