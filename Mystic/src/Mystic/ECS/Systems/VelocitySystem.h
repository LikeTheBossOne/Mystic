#pragma once
#include "Mystic/ECS/ComponentRegistry.h"
#include "Mystic/ECS/System.h"

namespace Mystic
{
	class VelocitySystem : public System
	{
		void OnUpdate(Scene* scene, entt::registry& registry, float deltaTime) override;
	};
}
