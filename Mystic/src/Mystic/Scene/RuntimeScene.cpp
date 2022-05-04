#include "RuntimeScene.h"

#include "Mystic/GFX/Camera.h"
#include "Mystic/GFX/Renderer.h"
#include "Mystic/ECS/Components/Renderable.h"
#include "Mystic/ECS/Components/TransformComponent.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "Mystic/ECS/Components/CameraComponent.h"
#include "Mystic/ECS/Components/SpriteRendererComponent.h"
#include "Mystic/GFX/Renderer2D.h"

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

	void RuntimeScene::OnUpdate()
	{
		/*Renderer3D::UseShaderProgram();

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), 1.f, 0.1f, 100.f);
		Renderer3D::SetProjectionMatrix(projection);

		glm::mat4 viewMat = _mainCamera->GetViewMatrix();
		Renderer3D::SetViewMatrix(viewMat);

		auto group = _registry->group<Renderable>(entt::get<TransformComponent>);
		group.each([](const Renderable& renderable, const TransformComponent& transform)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, transform.Position);
			glm::mat4 rotMatrix = glm::toMat4(transform.Rotation);
			model = model * rotMatrix;
			model = glm::scale(model, transform.Scale);

			std::string key = renderable.MeshKey;
			Renderer3D::RenderEnt(key, model);
		});*/

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
