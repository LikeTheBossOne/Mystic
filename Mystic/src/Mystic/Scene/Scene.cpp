#include "Scene.h"
#include "../ECS/Components/GUIDComponent.h"
#include "../ECS/Components/TagComponent.h"
#include "Mystic/Assets/AssetLibrary.h"
#include "Mystic/Assets/FBXImporter.h"
#include "Mystic/ECS/Components/CameraComponent.h"
#include "Mystic/ECS/Components/MeshRendererComponent.h"
#include "Mystic/ECS/Components/TransformComponent.h"
#include "Mystic/GameCode/GameCodeSystem.h"
#include "Mystic/Logging/Log.h"
#include "Mystic/Render/Renderer3D.h"

namespace Mystic
{
	Scene::Scene()
	{
		_registry = entt::registry();
		_assetLibrary = std::make_shared<AssetLibrary>();

		// std::unordered_map<FBXAssetType, std::unordered_map<std::string, std::string>> map;
		// FBXImporter::Import("../Game/assets/fbx/BaseCharacter.fbx", map);
		// for (auto filePair : map[FBXAssetType::MESH])
		// {
		// 	_mesh = Mesh::CreateMeshFromMystAsset(filePair.second);
		// }
		BufferLayout layout{
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
		);
	}

	Scene::~Scene()
	{
	}

	void Scene::SetName(const std::string& name)
	{
		_name = name;
	}

	std::string Scene::GetName() const
	{
		return _name;
	}

	Entity Scene::CreateEntity()
	{
		return CreateEntity("default");
	}

	Entity Scene::GetEntity(entt::entity id)
	{
		Entity entity{};
		if (!_registry.valid(id))
		{
			return entity;
		}

		GUIDComponent comp = _registry.get<GUIDComponent>(id);
		entity.EntId = id;
		entity.GUID = comp.GUID;
		entity.OwningScene = this;

		return entity;
	}

	void Scene::DestroyEntity(Entity& entity)
	{
		_registry.destroy(entity.EntId);
	}

	void Scene::EntityAddComponentByName(entt::entity e, std::string componentName)
	{
		GameCodeSystem::AddComponentFromString(componentName, e, _registry, this);
	}

	void Scene::ReloadGameCode()
	{
		GameCodeSystem::Reload(this);
	}

	void Scene::GameCodeImGui(entt::entity entity)
	{
		GameCodeSystem::ImGui(_registry, entity);
	}

	void Scene::ClearGameCode()
	{
		_registry = {};


		GameCodeSystem::FreeGameCodeLibrary(_registry);
	}

	void Scene::Clear()
	{
		//GameCodeSystem::FreeGameCodeLibrary(_registry);
		//_registry.clear();
	}

	void Scene::OnUpdate(float deltaTime)
	{
		GameCodeSystem::Update(deltaTime, this);
	}

	void Scene::OnEditorUpdate(float deltaTime)
	{
		GameCodeSystem::Update(deltaTime, this);
	}

	void Scene::OnRender()
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

	void Scene::OnRender(EditorCamera& editorCamera)
	{
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

		Renderer3D::BeginScene(editorCamera);

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

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		_viewportWidth = width;
		_viewportHeight = height;

		// Resize our non-FixedAspectRatio cameras
		auto view = _registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}

	}

	Entity Scene::CreateEntity(std::string name)
	{
		GUID entGuid;
		HRESULT result = CoCreateGuid(&entGuid);
		Log::Assert(result == S_OK, "GUID Creation failed");
		
		return CreateEntity(name, entGuid);
	}

	Entity Scene::CreateEntity(std::string name, GUID guid)
	{
		Entity ent(_registry.create(), guid, this);

		_registry.emplace<GUIDComponent>(ent.EntId, GUIDComponent{ guid });
		_registry.emplace<TagComponent>(ent.EntId, TagComponent{ name });
		_registry.emplace<TransformComponent>(ent.EntId);

		return ent;
	}
}
