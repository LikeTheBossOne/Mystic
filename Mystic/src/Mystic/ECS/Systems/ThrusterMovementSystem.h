#pragma once
#include "Mystic/ECS/System.h"

namespace Mystic
{
	class ThrusterMovementSystem : public System
	{
		void OnUpdate(Scene* scene, entt::registry& registry, float deltaTime) override;
	};
}
