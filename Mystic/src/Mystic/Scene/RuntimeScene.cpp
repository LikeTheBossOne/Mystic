#include "RuntimeScene.h"

#include "Mystic/Render/Camera.h"
#include "Mystic/Render/Renderer.h"
#include "Mystic/ECS/Components/Renderable.h"
#include "Mystic/ECS/Components/TransformComponent.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "Mystic/ECS/Components/CameraComponent.h"
#include "Mystic/ECS/Components/SpriteRendererComponent.h"
#include "Mystic/Render/Renderer2D.h"

#include "Mystic/ECS/SystemRegistry.h"

namespace Mystic
{

	RuntimeScene::RuntimeScene()
	{
		_registry = entt::registry();
		_mainCamera = std::make_shared<Camera>();
	}

	RuntimeScene::~RuntimeScene()
	{
	}

	void RuntimeScene::OnUpdate(float deltaTime)
	{
		for (auto system: SystemRegistry::s_systems)
		{
			if (system.second.second)
			{
				system.second.first->OnUpdate(this, _registry, deltaTime);
			}
		}

		// Render 2D
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto view = _registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, cameraTransform);

			auto group = _registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
			}

			Renderer2D::EndScene();
		}
	}
}
