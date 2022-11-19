#include "AccelerationSystem.h"

#include "glm/common.hpp"
#include "Mystic/ECS/Components/RigidBodyComponent.h"
#include "Mystic/ECS/Components/VelocityComponent.h"

void Mystic::AccelerationSystem::OnUpdate(Scene* scene, entt::registry& registry, float deltaTime)
{
	auto group = registry.group<const RigidBodyComponent>(entt::get<VelocityComponent>);

	group.each([this, deltaTime](const RigidBodyComponent& rigidBody, VelocityComponent& velocity)
		{
			velocity.Velocity = glm::clamp(velocity.Velocity + (rigidBody.Acceleration * deltaTime), -MAX_VELOCITY, MAX_VELOCITY);
		});
}
