#include "CharacterMovementSystem.h"

#include <iostream>

#include "Mystic/Core/Input.h"
#include "Mystic/ECS/Entity.h"
#include "Mystic/ECS/Components/CharacterComponent.h"
#include "Mystic/ECS/Components/VelocityComponent.h"
#include "Mystic/ECS/Components/ThrusterComponent.h"
#include "Mystic/Scene/Scene.h"

namespace Mystic
{
	void CharacterMovementSystem::OnUpdate(Scene* scene, entt::registry& registry, float deltaTime)
	{
		auto group = registry.group<const CharacterComponent>(entt::get<VelocityComponent>, entt::exclude<ThrusterComponent>);

		group.each([this](const CharacterComponent& character, VelocityComponent& velocity)
			{
				if (character.Active)
				{
					if (Input::IsKeyPressed(Key::D) || Input::IsKeyPressed(Key::A))
					{
						if (Input::IsKeyPressed(Key::D))
						{
							velocity.Velocity.x = INPUT_VELOCITY;
						}
						if (Input::IsKeyPressed(Key::A))
						{
							velocity.Velocity.x = -INPUT_VELOCITY;
						}
					}
					else
					{
						velocity.Velocity.x = 0;
					}
					

					if (Input::IsKeyPressed(Key::W) || Input::IsKeyPressed(Key::S))
					{
						if (Input::IsKeyPressed(Key::W))
						{
							velocity.Velocity.y = INPUT_VELOCITY;
						}
						if (Input::IsKeyPressed(Key::S))
						{
							velocity.Velocity.y = INPUT_VELOCITY;
						}
					}
					else
					{
						velocity.Velocity.y = 0;
					}
				}
			});
	}
}
