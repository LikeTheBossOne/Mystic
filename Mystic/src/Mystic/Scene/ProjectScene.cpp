//#include "ProjectScene.h"
//
//#include <fstream>
//#include <sstream>
//
//#include "RuntimeScene.h"
//#include "Mystic/Assets/AssetLibrary.h"
//#include "Mystic/Assets/FBXImporter.h"
//#include "Mystic/ECS/Components/GUIDComponent.h"
//#include "Mystic/Render/Camera.h"
//#include "Mystic/Render/Renderer.h"
//#include "Mystic/ECS/ComponentRegistry.h"
//#include "Mystic/ECS/Components/MeshRendererComponent.h"
//#include "Mystic/ECS/Components/TransformComponent.h"
//#include "Mystic/GameCode/GameCodeSystem.h"
//#include "Mystic/Render/Renderer2D.h"
//#include "Mystic/Render/Renderer3D.h"
//
//namespace Mystic
//{
//
//	ProjectScene::ProjectScene()
//	{
//		_registry = entt::registry();
//		_mainCamera = std::make_shared<Camera>();
//		_assetLibrary = std::make_shared<AssetLibrary>();
//
//		std::unordered_map<FBXAssetType, std::unordered_map<std::string, std::string>> map;
//		//FBXImporter::Import("../Game/assets/fbx/BaseCharacter.fbx", map);
//		for (auto filePair : map[FBXAssetType::MESH])
//		{
//			_mesh = Mesh::CreateMeshFromMystAsset(filePair.second);
//		}
//		BufferLayout layout{
//			{ ShaderDataType::Float3, "a_Position" },
//			{ ShaderDataType::Float2, "a_UV" },
//			{ ShaderDataType::Int,  "a_TexIndex" },
//			{ ShaderDataType::Int,    "a_EntityID" }
//		};
//		Renderer3D::RegisterBatch(
//			"3DTexture", 
//			"assets/shaders/3DTexture.glsl", 
//			"crateMesh",
//			"MystData/assets/crate1.mysta", 
//			layout
//		);
//		//_mesh = Mesh::CreateMeshFromMystAsset("MystData/assets/crate1.mysta");
//		//_texture = Texture2D::Create("crate", "assets/textures/crate_1.jpg");
//
//		//GameCodeSystem::ReloadGameCode(_registry);
//	}
//
//	ProjectScene::~ProjectScene()
//	{
//	}
//
//	Ref<RuntimeScene> ProjectScene::CreateRuntimeScene()
//	{
//		Ref<RuntimeScene> runtimeScene = std::make_shared<RuntimeScene>();
//
//		runtimeScene->_name = _name;
//		runtimeScene->_assetLibrary = _assetLibrary;
//
//		std::unordered_map<GUID, entt::entity> uuidToEntMap;
//
//		for (const auto ent : _registry.view<GUIDComponent>())
//		{
//			GUID guid = _registry.get<GUIDComponent>(ent).GUID;
//
//			entt::entity toEnt = runtimeScene->_registry.create();
//			uuidToEntMap[guid] = toEnt;
//		}
//
//		ComponentRegistry::InvokeCopy(runtimeScene->_registry, _registry, uuidToEntMap);
//
//		return runtimeScene;
//	}
//	
//	void ProjectScene::OnUpdate(float deltaTime)
//	{
//
//	}
//
//	void ProjectScene::OnRender(EditorCamera& camera)
//	{
//		//TODO: Before every render, check and see if any batches need to be registered. In the future, we should find a way to make this cost not at runtime.
//		auto group = _registry.group<TransformComponent, MeshRendererComponent>();
//		for (auto entity : group)
//		{
//			auto [transform, meshRenderer] = group.get<TransformComponent, MeshRendererComponent>(entity);
//
//			Log::Assert(_assetLibrary->Shaders.contains(meshRenderer.ShaderName), "Asset Library missing shader: %s", meshRenderer.ShaderName.c_str());
//			Log::Assert(_assetLibrary->Textures.contains(meshRenderer.TextureName), "Asset Library missing texture: %s", meshRenderer.TextureName.c_str());
//			Log::Assert(_assetLibrary->Meshes.contains(meshRenderer.MeshName), "Asset Library missing mesh: %s", meshRenderer.MeshName.c_str());
//
//			if (!Renderer3D::BatchExists(meshRenderer.ShaderName, meshRenderer.MeshName))
//			{
//				Renderer3D::RegisterBatch(_assetLibrary->Shaders[meshRenderer.ShaderName], _assetLibrary->Meshes[meshRenderer.MeshName]);
//			}
//		}
//
//		Renderer3D::BeginScene(camera);
//
//		for (auto entity : group)
//		{
//			auto [transform, meshRenderer] = group.get<TransformComponent, MeshRendererComponent>(entity);
//
//			Renderer3D::DrawModel(
//				transform.GetTransform(),
//				meshRenderer.ShaderName,
//				meshRenderer.MeshName,
//				_assetLibrary->Textures[meshRenderer.TextureName],
//				(int)entity
//			);
//		}
//
//		Renderer3D::EndScene();
//	}
//}
