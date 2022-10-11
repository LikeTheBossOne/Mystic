#include "RuntimeScene.h"

#include "Mystic/Render/Camera.h"
#include "Mystic/Render/Renderer.h"
#include "Mystic/ECS/Components/Renderable.h"
#include "Mystic/ECS/Components/TransformComponent.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "Mystic/Assets/AssetLibrary.h"
#include "Mystic/ECS/Components/CameraComponent.h"
#include "Mystic/ECS/Components/SpriteRendererComponent.h"
#include "Mystic/Render/Renderer2D.h"

#include "Mystic/ECS/SystemRegistry.h"
#include "Mystic/ECS/Components/MeshRendererComponent.h"
#include "Mystic/GameCode/GameCodeSystem.h"
#include "Mystic/Render/Renderer3D.h"

namespace Mystic
{

	RuntimeScene::RuntimeScene()
	{
		_registry = entt::registry();
		_mainCamera = std::make_shared<Camera>();
		_assetLibrary = std::make_shared<AssetLibrary>();

		/*BufferLayout layout{
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_UV" },
			{ ShaderDataType::Int,  "a_TexIndex" },
			{ ShaderDataType::Int,    "a_EntityID" }
		};
		Renderer3D::RegisterBatch(
			"3DTexture",
			"assets/shaders/3DTexture.glsl",
			"crateMesh",
			"MystData/assets/crate1.mysta",
			layout
		);*/

		//ReloadGameCode();
	}

	RuntimeScene::~RuntimeScene()
	{
	}

	void RuntimeScene::OnUpdate(float deltaTime)
	{
		GameCodeSystem::Update(deltaTime, this);
	}

	void RuntimeScene::OnRender()
	{
		// OnRender 2D
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
					camera.Camera.SetPerspectiveVerticalFOV(30);
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		//TODO: Before every render, check and see if any batches need to be registered. In the future, we should find a way to make this cost not at runtime.
		auto group = _registry.group<TransformComponent, MeshRendererComponent>();
		for (auto entity : group)
		{
			auto [transform, meshRenderer] = group.get<TransformComponent, MeshRendererComponent>(entity);

			Log::Assert(_assetLibrary->Shaders.contains(meshRenderer.ShaderName), "Asset Library missing shader: %s", meshRenderer.ShaderName.c_str());
			Log::Assert(_assetLibrary->Textures.contains(meshRenderer.TextureName), "Asset Library missing texture: %s", meshRenderer.TextureName.c_str());
			Log::Assert(_assetLibrary->Meshes.contains(meshRenderer.MeshName), "Asset Library missing mesh: %s", meshRenderer.MeshName.c_str());

			if (!Renderer3D::BatchExists(meshRenderer.ShaderName, meshRenderer.MeshName))
			{
				Renderer3D::RegisterBatch(_assetLibrary->Shaders[meshRenderer.ShaderName], _assetLibrary->Meshes[meshRenderer.MeshName]);
			}
		}

		if (mainCamera)
		{
			Renderer3D::BeginScene(*mainCamera, cameraTransform);

			for (auto entity : group)
			{
				auto [transform, meshRenderer] = group.get<TransformComponent, MeshRendererComponent>(entity);

				Renderer3D::DrawModel(
					transform.GetTransform(),
					meshRenderer.ShaderName,
					meshRenderer.MeshName,
					_assetLibrary->Textures[meshRenderer.TextureName],
					(int)entity
				);
			}

			Renderer3D::EndScene();
		}
	}
}
