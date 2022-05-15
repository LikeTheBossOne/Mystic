#pragma once
#include "Mystic/ECS/System.h"

class WindowLoopSystem : public Mystic::System
{
public:
	void OnUpdate(Mystic::Scene* scene, entt::registry& registry, float deltaTime) override;
};
