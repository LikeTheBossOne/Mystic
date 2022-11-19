#pragma once
#include "Mystic/ECS/System.h"

namespace Mystic
{
	class CharacterMovementSystem : public System
	{
	public:
		void OnUpdate(Scene* scene, entt::registry& registry, float deltaTime) override;

	private:
		const float INPUT_VELOCITY = 5.f;
	};
}
