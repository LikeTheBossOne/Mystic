#include "WindowLoopSystem.h"

#include "Mystic/ECS/Components/CameraComponent.h"
#include "Mystic/ECS/Components/TransformComponent.h"
#include "Mystic/Scene/Scene.h"
#include "Mystic/Scene/SceneCamera.h"

void WindowLoopSystem::OnUpdate(Mystic::Scene* scene, entt::registry& registry, float deltaTime)
{
	auto cameraView = registry.view<Mystic::CameraComponent>();

	Mystic::SceneCamera* mainCam = nullptr;

	for (auto [entity, camera] : cameraView.each())
	{
		if (camera.Primary)
		{
			mainCam = &camera.Camera;
			break;
		}
	}

	if (mainCam)
	{
		if (mainCam->GetProjectionType() == Mystic::SceneCamera::ProjectionType::Orthographic)
		{
			float height = mainCam->GetOrthographicSize();
			float width = height * mainCam->GetAspectRatio();

			auto view = registry.view<Mystic::TransformComponent>();
			for (auto [ent, transform] : view.each())
			{
				if (transform.Position.x > width / 2)
				{
					transform.Position.x = -width / 2;
				}
				else if (transform.Position.x < -width / 2)
				{
					transform.Position.x = width / 2;
				}

				if (transform.Position.y > height / 2)
				{
					transform.Position.y = -height / 2;
				}
				else if (transform.Position.y < -height / 2)
				{
					transform.Position.y = height / 2;
				}
			}
		}
	}

	
}
