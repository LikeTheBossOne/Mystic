#include "ProjectSerializer.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "ProjectScene.h"
#include "../ECS/Components/TransformComponent.h"
#include "../ECS/Components/Renderable.h"
#include "Mystic/ECS/Components/CameraComponent.h"
#include "Mystic/ECS/Components/CharacterComponent.h"
#include "Mystic/ECS/Components/GUIDComponent.h"
#include "Mystic/ECS/Components/MeshRendererComponent.h"
#include "Mystic/ECS/Components/SpriteRendererComponent.h"
#include "Mystic/ECS/Components/TagComponent.h"
#include "Mystic/ECS/Components/VelocityComponent.h"
#include "yaml-cpp/yaml.h"
#include "Mystic/Assets/AssetLibrary.h"
#include "Mystic/GameCode/GameCodeSystem.h"

namespace YAML
{
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::quat>
	{
		static Node encode(const glm::quat& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::quat& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<Mystic::BufferLayout>
	{
		static Node encode(const Mystic::BufferLayout& rhs)
		{
			Node node;
			for (auto element : rhs.GetElements())
			{
				Node mapNode;
				mapNode.force_insert("Name", element.Name);
				mapNode.force_insert("Type", ShaderDataTypeName(element.Type));

				node.push_back(mapNode);
			}
			return node;
		}

		static bool decode(const Node& node, Mystic::BufferLayout& rhs)
		{
			if (!node.IsSequence())
				return false;

			std::vector<Mystic::BufferElement> elements;
			for (auto nodeElement : node)
			{
				elements.push_back({ 
					Mystic::ShaderDataTypeFromName(nodeElement["Type"].as<std::string>()),
					nodeElement["Name"].as<std::string>()
				});
			}
			rhs = { elements };

			return true;
		}
	};
}

namespace Mystic
{
	YAML::Emitter& operator << (YAML::Emitter& out, const glm::vec3& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator << (YAML::Emitter & out, const glm::vec4 & v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator << (YAML::Emitter& out, const glm::quat& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator << (YAML::Emitter& out, const BufferLayout& v) {
		out << YAML::BeginSeq;
		for (auto element : v.GetElements())
		{
			out << YAML::BeginMap;

			out << YAML::Key << "Name";
			out << YAML::Value << element.Name;
			out << YAML::Key << "Type";
			out << YAML::Value << ShaderDataTypeName(element.Type);

			out << YAML::EndMap;
		}
		out << YAML::EndSeq;
		return out;
	}

	ProjectSerializer::ProjectSerializer(Ref<Scene> scene)
	{
		_scene = scene;
		_strCount = 0;
	}

	GUID StringToGuid(std::string& str)
	{
		GUID guid;
		unsigned long p0;
		int p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;

		sscanf(str.c_str(), "%08lx-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
			&p0, &p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10);

		guid.Data1 = p0;
		guid.Data2 = p1;
		guid.Data3 = p2;
		guid.Data4[0] = p3;
		guid.Data4[1] = p4;
		guid.Data4[2] = p5;
		guid.Data4[3] = p6;
		guid.Data4[4] = p7;
		guid.Data4[5] = p8;
		guid.Data4[6] = p9;
		guid.Data4[7] = p10;

		return guid;
	}

	void ProjectSerializer::SerializeProject(std::string& filePath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;

		out << YAML::Key << "Scene";
		out << YAML::Value << _scene->GetName();
		out << YAML::Key << "Entities";
		out << YAML::Value << YAML::BeginSeq;

		_scene->_registry.each([this, &out](const entt::entity ent)
			{
				SerializeEntity(out, ent);
			});

		out << YAML::EndSeq;

		SerializeAssets(out);
		
		out << YAML::EndMap;

		std::ofstream fOut(filePath);
		fOut << out.c_str();
	}
	
	bool ProjectSerializer::DeserializeProject(const std::string& filePath)
	{

		YAML::Node sceneRoot;
		try
		{
			sceneRoot = YAML::LoadFile(filePath);
		}
		catch (YAML::BadFile& ex)
		{
			Log::Assert(false, "could not load yaml file: %s", filePath.c_str());
			return false;
		}

		if (!sceneRoot["Scene"])
			return false;
		
		_scene->_name = sceneRoot["Scene"].as<std::string>();

		YAML::Node entities = sceneRoot["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				DeserializeEntity(entity);
			}
		}

		DeserializeAssets(sceneRoot);
		
		return true;
	}

	void ProjectSerializer::SerializeGameCodeEnts(std::string& filePath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		
		out << YAML::Key << "Entities";
		out << YAML::Value << YAML::BeginSeq;

		_scene->_registry.each([this, &out](const entt::entity ent)
			{
				SerializeEntity(out, ent);
			});

		out << YAML::EndSeq;

		out << YAML::EndMap;

		std::ofstream fOut(filePath);
		fOut << out.c_str();
	}

	bool ProjectSerializer::DeserializeGameCodeEnts(std::string& filePath)
	{
		YAML::Node gameCodeRoot;
		try
		{
			gameCodeRoot = YAML::LoadFile(filePath);
		}
		catch (YAML::BadFile& ex)
		{
			Log::Assert(false, "could not load yaml file: %s", filePath.c_str());
			return false;
		}

		YAML::Node entities = gameCodeRoot["Entities"];

		if (entities)
		{
			for (auto entity : entities)
			{
				DeserializeEntity(entity);
			}
		}

		return true;
	}

	void ProjectSerializer::SerializeAssets(YAML::Emitter& out)
	{
		out << YAML::Key << "Assets";
		out << YAML::Value << YAML::BeginMap;

		out << YAML::Key << "Meshes";
		out << YAML::Value << YAML::BeginSeq;
		for (auto pair : _scene->_assetLibrary->Meshes)
		{
			out << YAML::BeginMap;

			out << YAML::Key << "Path";
			out << YAML::Value << pair.first;

			out << YAML::Key << "Name";
			out << YAML::Value << pair.second->GetName();

			out << YAML::EndMap;
			
		}
		out << YAML::EndSeq;

		out << YAML::Key << "Shaders";
		out << YAML::Value << YAML::BeginSeq;
		for (auto pair : _scene->_assetLibrary->Shaders)
		{
			out << YAML::BeginMap;

			out << YAML::Key << "Path";
			out << YAML::Value << pair.first;

			out << YAML::Key << "Name";
			out << YAML::Value << pair.second->GetName();

			out << YAML::Key << "BufferLayout";
			out << YAML::Value << pair.second->GetBufferLayout();

			out << YAML::EndMap;
		}
		out << YAML::EndSeq;

		out << YAML::Key << "Textures";
		out << YAML::Value << YAML::BeginSeq;
		for (auto pair : _scene->_assetLibrary->Textures)
		{
			out << YAML::BeginMap;

			out << YAML::Key << "Path";
			out << YAML::Value << pair.first;

			out << YAML::Key << "Name";
			out << YAML::Value << pair.second->GetName();

			out << YAML::EndMap;
		}
		out << YAML::EndSeq;

		out << YAML::EndMap;
	}

	std::string GuidToString(GUID& guid)
	{
		char guid_cstr[39];
		snprintf(guid_cstr, sizeof(guid_cstr),
			"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
			guid.Data1, guid.Data2, guid.Data3,
			guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
			guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);

		return std::string(guid_cstr);
	}
	

	void ProjectSerializer::SerializeEntity(YAML::Emitter& out, entt::entity entity)
	{
		out << YAML::BeginMap;

		GUIDComponent guid = _scene->_registry.get<GUIDComponent>(entity);
		out << YAML::Key << "Entity";
		std::string guidStr = GuidToString(guid.GUID);
		out << YAML::Value << guidStr;

		TagComponent tag = _scene->_registry.get<TagComponent>(entity);
		out << YAML::Key << "TagComponent" << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "Tag";
		out << YAML::Value << tag.Tag;
		out << YAML::EndMap;


		TransformComponent* transformComponent;
		if ((transformComponent = _scene->_registry.try_get<TransformComponent>(entity)))
		{
			out << YAML::Key << "TransformComponent" << YAML::Value << YAML::BeginMap;

			out << YAML::Key << "Position" << YAML::Value << transformComponent->Position;
			out << YAML::Key << "Rotation" << YAML::Value << transformComponent->Rotation;
			out << YAML::Key << "Scale" << YAML::Value << transformComponent->Scale;

			out << YAML::EndMap;
		}

		Renderable* renderable;
		if ((renderable = _scene->_registry.try_get<Renderable>(entity)))
		{
			out << YAML::Key << "Renderable" << YAML::Value << YAML::BeginMap;

			out << YAML::Key << "Mesh" << YAML::Value << renderable->MeshKey;

			out << YAML::EndMap;
		}

		CameraComponent* cameraComponent;
		if ((cameraComponent = _scene->_registry.try_get<CameraComponent>(entity)))
		{
			out << YAML::Key << "CameraComponent" << YAML::BeginMap;

			auto& camera = cameraComponent->Camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap;
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
			out << YAML::EndMap;

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent->Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent->FixedAspectRatio;

			out << YAML::EndMap;
		}

		SpriteRendererComponent* spriteRendererComponent;
		if ((spriteRendererComponent = _scene->_registry.try_get<SpriteRendererComponent>(entity)))
		{
			out << YAML::Key << "SpriteRendererComponent" << YAML::BeginMap;
			
			out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent->Color;

			out << YAML::EndMap;
		}

		VelocityComponent* velocityComponent;
		if ((velocityComponent = _scene->_registry.try_get<VelocityComponent>(entity)))
		{
			out << YAML::Key << "VelocityComponent" << YAML::BeginMap;

			out << YAML::Key << "Velocity" << YAML::Value << velocityComponent->Velocity;

			out << YAML::EndMap;
		}

		CharacterComponent* characterComponent;
		if ((characterComponent = _scene->_registry.try_get<CharacterComponent>(entity)))
		{
			out << YAML::Key << "CharacterComponent" << YAML::BeginMap;

			out << YAML::Key << "Active" << YAML::Value << characterComponent->Active;

			out << YAML::EndMap;
		}

		MeshRendererComponent* meshRendererComponent;
		if ((meshRendererComponent = _scene->_registry.try_get<MeshRendererComponent>(entity)))
		{
			out << YAML::Key << "MeshRendererComponent" << YAML::BeginMap;

			out << YAML::Key << "MeshName" << YAML::Value << meshRendererComponent->MeshName;
			out << YAML::Key << "ShaderName" << YAML::Value << meshRendererComponent->ShaderName;
			out << YAML::Key << "TextureName" << YAML::Value << meshRendererComponent->TextureName;
			out << YAML::Key << "Color" << YAML::Value << meshRendererComponent->Color;

			out << YAML::EndMap;
		}

		GameCodeSystem::SerializeEntity(_scene->_registry, out, entity);

		out << YAML::EndMap;
	}

	void ProjectSerializer::DeserializeAssets(YAML::Node& root)
	{
		if (!root["Assets"])
			return;

		auto assetNode = root["Assets"];

		if (assetNode["Meshes"])
		{
			for (auto mesh : assetNode["Meshes"])
			{
				if (!mesh["Path"] || !mesh["Name"])
					continue;

				_scene->_assetLibrary->Meshes[mesh["Name"].as<std::string>()] = 
					Mesh::CreateMeshFromMystAsset(mesh["Name"].as<std::string>(), mesh["Path"].as<std::string>());
			}
		}

		if (assetNode["Shaders"])
		{
			for (auto shader : assetNode["Shaders"])
			{
				if (!shader["Path"] || !shader["Name"] || !shader["BufferLayout"])
					continue;

				BufferLayout bf = shader["BufferLayout"].as<BufferLayout>();
				_scene->_assetLibrary->Shaders[shader["Name"].as<std::string>()] =
					Shader::Create(
						shader["Name"].as<std::string>(), 
						shader["Path"].as<std::string>(), 
						bf
					);
			}
		}

		if (assetNode["Textures"])
		{
			for (auto texture : assetNode["Textures"])
			{
				if (!texture["Path"] || !texture["Name"])
					continue;
				
				_scene->_assetLibrary->Textures[texture["Name"].as<std::string>()] =
					Texture2D::Create(
						texture["Name"].as<std::string>(),
						texture["Path"].as<std::string>()
					);
			}
		}
	}

	void ProjectSerializer::DeserializeEntity(YAML::detail::iterator_value& entity)
	{
		auto guidStr = entity["Entity"].as<std::string>();
		GUID guid = StringToGuid(guidStr);

		auto tag = entity["TagComponent"]["Tag"].as<std::string>();

		Entity deserializedEntity = _scene->CreateEntity(tag, guid);

		auto transformComponent = entity["TransformComponent"];
		if (transformComponent)
		{
			// Entities always have transforms
			TransformComponent& tc = _scene->EntityGetComponent<TransformComponent>(deserializedEntity.EntId);
			tc.Position = transformComponent["Position"].as<glm::vec3>();
			tc.Rotation = transformComponent["Rotation"].as<glm::quat>();
			tc.Scale = transformComponent["Scale"].as<glm::vec3>();
		}

		auto cameraComponent = entity["CameraComponent"];
		if (cameraComponent)
		{
			CameraComponent cc;

			YAML::Node cameraProps = cameraComponent["Camera"];
			cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

			cc.Camera.SetPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
			cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
			cc.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

			cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
			cc.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
			cc.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

			cc.Primary = cameraComponent["Primary"].as<bool>();
			cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();

			_scene->_registry.emplace<CameraComponent>(deserializedEntity.EntId, cc);
		}

		auto spriteRendererComponent = entity["SpriteRendererComponent"];
		if (spriteRendererComponent)
		{
			SpriteRendererComponent src;
			src.Color = spriteRendererComponent["Color"].as<glm::vec4>();

			_scene->_registry.emplace<SpriteRendererComponent>(deserializedEntity.EntId, src);
		}

		auto renderableComponent = entity["Renderable"];
		if (renderableComponent)
		{
			Renderable rc;
			rc.MeshKey = renderableComponent["Mesh"].as<std::string>();

			_scene->_registry.emplace<Renderable>(deserializedEntity.EntId, rc);
		}

		auto velocityComponent = entity["VelocityComponent"];
		if (velocityComponent)
		{
			VelocityComponent vc;
			vc.Velocity = velocityComponent["Velocity"].as<glm::vec3>();

			_scene->_registry.emplace<VelocityComponent>(deserializedEntity.EntId, vc);
		}

		auto characterComponent = entity["CharacterComponent"];
		if (characterComponent)
		{
			CharacterComponent cc;
			cc.Active = characterComponent["Active"].as<bool>();

			_scene->_registry.emplace<CharacterComponent>(deserializedEntity.EntId, cc);
		}

		auto meshRendererComponent = entity["MeshRendererComponent"];
		if (meshRendererComponent)
		{
			MeshRendererComponent mrc;
			mrc.MeshName = meshRendererComponent["MeshName"].as<std::string>();
			mrc.ShaderName = meshRendererComponent["ShaderName"].as<std::string>();
			mrc.TextureName = meshRendererComponent["TextureName"].as<std::string>();
			mrc.Color = meshRendererComponent["Color"].as<glm::vec4>();

			_scene->_registry.emplace<MeshRendererComponent>(deserializedEntity.EntId, mrc);
		}

		GameCodeSystem::DeserializeEntity(_scene->_registry, entity, deserializedEntity.EntId, _scene.get());
	}
}
