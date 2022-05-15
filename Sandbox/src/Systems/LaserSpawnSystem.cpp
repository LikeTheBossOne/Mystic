#include "LaserSpawnSystem.h"

#include "Components/LaserComponent.h"
#include "Components/PlayerComponent.h"
#include "Mystic/Core/Input.h"
#include "Mystic/ECS/Entity.h"
#include "Mystic/ECS/Components/SpriteRendererComponent.h"
#include "Mystic/ECS/Components/TransformComponent.h"
#include "Mystic/ECS/Components/VelocityComponent.h"
#include "Mystic/Scene/Scene.h"

void LaserSpawnSystem::OnUpdate(Mystic::Scene* scene, entt::registry& registry, float deltaTime)
{
	auto playerGroup = registry.group<PlayerComponent>(entt::get<Mystic::TransformComponent>);

	for (auto [entity, player, trans] : playerGroup.each())
	{
		player.TimeSinceShotLast += deltaTime;
		if (player.TimeSinceShotLast > 100) player.TimeSinceShotLast = 100;
		if (Mystic::Input::IsKeyPressed(Mystic::Key::Space) && player.TimeSinceShotLast > 5.f)
		{
			Mystic::Entity laserEnt = scene->CreateEntity("laser");

			Mystic::TransformComponent laserTrans = trans;
			laserTrans.Scale *= 0.25f;
			scene->EntityGetComponent<Mystic::TransformComponent>(laserEnt) = laserTrans;

			glm::vec3 eulerAngles = glm::eulerAngles(trans.Rotation);
			glm::vec3 vel;
			vel.x = cos(eulerAngles.z) * 1.5f;
			vel.y = sin(eulerAngles.z) * 1.5f;
			vel.z = 0;
			scene->EntityAddComponent<Mystic::VelocityComponent>(laserEnt, vel);

			scene->EntityAddComponent<LaserComponent>(laserEnt);

			glm::vec4 color(0, 1, 0, 1);
			scene->EntityAddComponent<Mystic::SpriteRendererComponent>(laserEnt, color);

			player.TimeSinceShotLast = 0.f;
		}
	}
}
