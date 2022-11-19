#pragma once
#include "Mystic/ECS/System.h"

namespace Mystic
{
	class AccelerationSystem : public System
	{
	public:
		void OnUpdate(Scene* scene, entt::registry& registry, float deltaTime) override;

	private:
		const float MAX_VELOCITY = 10.f;
	};
}
