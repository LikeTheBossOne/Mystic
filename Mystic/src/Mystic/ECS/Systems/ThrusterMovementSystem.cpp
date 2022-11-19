#include "ThrusterMovementSystem.h"

#include "Mystic/Core/Input.h"
#include "Mystic/ECS/Components/CharacterComponent.h"
#include "Mystic/ECS/Components/RigidBodyComponent.h"
#include "Mystic/ECS/Components/ThrusterComponent.h"
#include "Mystic/ECS/Components/TransformComponent.h"

namespace Mystic
{
	void ThrusterMovementSystem::OnUpdate(Scene* scene, entt::registry& registry, float deltaTime)
	{
		auto group = registry.group<const ThrusterComponent>(entt::get<const CharacterComponent, RigidBodyComponent, TransformComponent>);

		glm::vec3 rotationAxis(0, 0, 1);
		for (auto [entId, thruster, character, rigidBody, transform] : group.each())
		{
			if (character.Active)
			{
				if (Input::IsKeyPressed(Key::A))
				{
					transform.Rotation = glm::rotate(transform.Rotation, thruster.RotationRate * deltaTime, rotationAxis);
				}
				if (Input::IsKeyPressed(Key::D))
				{
					transform.Rotation = glm::rotate(transform.Rotation, -thruster.RotationRate * deltaTime, rotationAxis);
				}

				if (Input::IsKeyPressed(Key::W))
				{
					glm::vec3 dirVector = glm::eulerAngles(transform.Rotation);
					rigidBody.Acceleration.x = glm::cos(dirVector.z) * thruster.Thrust;
					rigidBody.Acceleration.y = glm::sin(dirVector.z) * thruster.Thrust;
				}
				else
				{
					rigidBody.Acceleration.x = 0;
					rigidBody.Acceleration.y = 0;
				}
			}
		}

	}
}
