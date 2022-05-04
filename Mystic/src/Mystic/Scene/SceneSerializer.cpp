#include "SceneSerializer.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "ProjectScene.h"
#include "../ECS/Components/TransformComponent.h"
#include "../ECS/Components/Renderable.h"
#include "Mystic/ECS/Components/CameraComponent.h"
#include "Mystic/ECS/Components/GUIDComponent.h"
#include "Mystic/ECS/Components/SpriteRendererComponent.h"
#include "Mystic/ECS/Components/TagComponent.h"
#include "yaml-cpp/yaml.h"

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

	SceneSerializer::SceneSerializer(Ref<ProjectScene> scene)
	{
		_scene = scene;
		_strCount = 0;
	}

	void SceneSerializer::SerializeScene(std::string& filePath)
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

		out << YAML::EndMap;

		std::ofstream fOut(filePath);
		fOut << out.c_str();
	}

	typedef std::vector<unsigned char> bytes;
	bytes HexToBytes(const std::string& hex) {
		bytes bytesArr;

		for (unsigned int i = 0; i < hex.length(); i += 2) {
			std::string byteString = hex.substr(i, 2);
			char byte = (char)strtol(byteString.c_str(), NULL, 16);
			bytesArr.push_back(byte);
		}

		return bytesArr;
	}

	bool SceneSerializer::DeserializeScene(const std::string& filePath)
	{

		YAML::Node sceneRoot;
		try
		{
			sceneRoot = YAML::LoadFile(filePath);
		}
		catch (YAML::BadFile& ex)
		{
			assert(false, "could not load yaml file");
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
				GUID guid;
				auto guidStr = entity["Entity"].as<std::string>();
				bytes guidBytes = HexToBytes(guidStr);
				std::memcpy(&guid, guidBytes.data(), sizeof(guid));

				auto tag = entity["TagComponent"]["Tag"].as<std::string>();

				Entity deserializedEntity = _scene->CreateEntity(tag, guid);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					// Entities always have transforms
					TransformComponent tc;
					tc.Position = transformComponent["Position"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::quat>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();

					_scene->_registry.emplace<TransformComponent>(deserializedEntity.EntId, tc);
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
			}
		}
		return true;
	}
	
	std::string BytesToStr(const bytes& in)
	{
		bytes::const_iterator from = in.cbegin();
		bytes::const_iterator to = in.cend();
		std::ostringstream oss;
		for (; from != to; ++from)
			oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(*from);
		return oss.str();
	}

	bytes longToBytes(const unsigned long paramLong)
	{
		bytes arrayOfByte(8);
		for (int i = 0; i < 8; i++)
			arrayOfByte[7 - i] = (paramLong >> (i * 8));
		return arrayOfByte;
	}

	bytes shortToBytes(const unsigned short paramShort)
	{
		bytes arrayOfByte(2);
		for (int i = 0; i < 2; i++)
			arrayOfByte[1 - i] = (paramShort >> (i * 8));
		return arrayOfByte;
	}

	std::string LongBytesToStr(const unsigned long paramInt)
	{
		bytes longBytes = longToBytes(paramInt);
		bytes::const_iterator from = longBytes.cbegin();
		bytes::const_iterator to = longBytes.cend();
		std::ostringstream oss;
		for (; from != to; ++from)
			oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(*from);
		return oss.str();
	}

	std::string ShortBytesToStr(const unsigned short paramShort)
	{
		bytes shortBytes = shortToBytes(paramShort);
		bytes::const_iterator from = shortBytes.cbegin();
		bytes::const_iterator to = shortBytes.cend();
		std::ostringstream oss;
		for (; from != to; ++from)
			oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(*from);
		return oss.str();
	}
	

	void SceneSerializer::SerializeEntity(YAML::Emitter& out, entt::entity entity)
	{
		out << YAML::BeginMap;

		GUIDComponent guid = _scene->_registry.get<GUIDComponent>(entity);
		out << YAML::Key << "GUID";
		out << YAML::Value << LongBytesToStr(guid.GUID.Data1) << ShortBytesToStr(guid.GUID.Data2) << ShortBytesToStr(guid.GUID.Data3);
		out << BytesToStr(std::vector<unsigned char>(guid.GUID.Data4, guid.GUID.Data4 + 8));

		TagComponent tag = _scene->_registry.get<TagComponent>(entity);
		out << YAML::Key << "Tag";
		out << YAML::Value << tag.Name;


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

		out << YAML::EndMap;
	}
}
