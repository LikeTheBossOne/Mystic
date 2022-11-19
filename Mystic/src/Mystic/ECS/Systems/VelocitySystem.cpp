#include "VelocitySystem.h"

#include "Mystic/ECS/ComponentRegistry.h"
#include "Mystic/ECS/Components/TransformComponent.h"
#include "Mystic/ECS/Components/VelocityComponent.h"
#include "Mystic/Scene/Scene.h"

namespace Mystic
{
	void VelocitySystem::OnUpdate(Scene* scene, entt::registry& registry, float deltaTime)
	{
		auto group = registry.group<const VelocityComponent>(entt::get<TransformComponent>);

		for (entt::entity ent : group)
		{
			auto [velocity, transform] = group.get(ent);

			transform.Position += (velocity.Velocity * deltaTime);
		}
	}
}
