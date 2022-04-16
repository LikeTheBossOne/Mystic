#include "SceneSerializer.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "ProjectScene.h"
#include "../ECS/Components/Transform.h"
#include "../ECS/Components/Renderable.h"

namespace Mystic
{

	SceneSerializer::SceneSerializer(Ref<ProjectScene> scene)
	{
		_scene = scene;
		_strCount = 0;
	}

	//void SceneSerializer::SerializeScene(std::string& filePath)
	//{
	//	FILE* outFile;
	//	errno_t err;
	//	if ((err = fopen_s(&outFile, filePath.c_str(), "w") != 0))
	//	{
	//		//TODO: Write Error
	//		return;
	//	}

	//	Tree tree;
	//	NodeRef root = tree.rootref();
	//	root |= MAP;

	//	std::string name = "name";
	//	root["ProjectScene"] = to_substr(name);

	//	NodeRef baseEntsNode = root["Entities"];
	//	baseEntsNode |= SEQ;

	//	uint16_t strCount = 0;
	//	_strings.clear();

	//	_scene->GetRegistry()->each([this, &baseEntsNode, &strCount](const entt::entity ent)
	//		{
	//			SerializeEntity(baseEntsNode, strCount, ent);
	//		});

	//	emit(tree, outFile);

	//	fclose(outFile);
	//}


	//void SceneSerializer::SerializeEntity(NodeRef& entsNode, uint16_t& strCount, entt::entity entity)
	//{
	//	Ref<entt::registry> reg = _scene->GetRegistry();

	//	NodeRef entNode = entsNode.append_child();
	//	entNode |= MAP;

	//	entNode["GUID"] << static_cast<uint32_t>(entity);

	//	Transform* transform = NULL;
	//	if ((transform = reg->try_get<Transform>(entity)))
	//	{
	//		NodeRef transformNode = entNode["Transform"];
	//		transformNode |= MAP;

	//		std::stringstream tBuf;
	//		tBuf << "" << transform->Position.x << "," << transform->Position.y << "," << transform->Position.z;
	//		std::string str1 = tBuf.str();
	//		SerializeString(str1, transformNode, "Position");


	//		std::stringstream rBuf;
	//		rBuf << "" << transform->Rotation.x << "," << transform->Rotation.y << "," << transform->Rotation.z << "," << transform->Rotation.w;
	//		std::string str2 = rBuf.str();
	//		SerializeString(str2, transformNode, "Rotation");

	//		std::stringstream sBuf;
	//		sBuf << "" << transform->Scale.x << "," << transform->Scale.y << "," << transform->Scale.z;
	//		std::string str3 = sBuf.str();
	//		SerializeString(str3, transformNode, "Scale");
	//	}

	//	Renderable* renderable = NULL;
	//	if ((renderable = reg->try_get<Renderable>(entity)))
	//	{
	//		NodeRef renderableNode = entNode["Renderable"];
	//		renderableNode |= MAP;

	//		renderableNode["MeshKey"] = to_substr(renderable->MeshKey);
	//	}
	//}


	//void SceneSerializer::SerializeString(std::string& str, c4::yml::NodeRef& node, const char* entChild)
	//{
	//	assert(++_strCount < MAX_STRINGS, "Max serialization string limit was surpassed");

	//	const auto refStr1 = new std::string();
	//	*refStr1 = str;

	//	_strings.emplace_back();

	//	node[to_csubstr(entChild)] = to_substr(*refStr1);
	//}


	//void SceneSerializer::DeserializeScene(std::string& filePath)
	//{
	//	std::string contents = GetFileContents(filePath);

	//	assert(contents.length() > 0, "File" << _scenePath << " has nothing in it or does not exist");

	//	const Tree tree = parse_in_place(to_substr(contents));
	//	const NodeRef root = tree.rootref();
	//	if (!root.has_child("ProjectScene"))
	//	{
	//		assert(false, "ProjectScene tag was missing from file");
	//		return;
	//	}

	//	const NodeRef sceneNode = root["ProjectScene"];
	//	const std::string name(sceneNode.has_val() ? sceneNode.val() : "Untitled");
	//	_scene->SetName(name);

	//	if (!root.has_child("Entities"))
	//	{
	//		return;
	//	}

	//	const NodeRef entitiesNode = root["Entities"];
	//	for (const NodeRef& ref : entitiesNode)
	//	{
	//		DeserializeEntity(ref);
	//	}
	//}

	//void SceneSerializer::DeserializeEntity(const NodeRef& entityNode)
	//{
	//	entt::entity ent = _scene->GetRegistry()->create();

	//	if (entityNode.has_child("Transform"))
	//	{
	//		NodeRef transformNode = entityNode["Transform"];

	//		Transform t{};

	//		if (transformNode.has_child("Position"))
	//		{
	//			const csubstr posStr = transformNode["Position"].val();
	//			
	//			DeserializeVec3f(posStr, &t.Position);
	//		}
	//		if (transformNode.has_child("Rotation"))
	//		{
	//			const csubstr posStr = transformNode["Rotation"].val();

	//			DeserializeQuat(posStr, &t.Rotation);
	//		}
	//		if (transformNode.has_child("Scale"))
	//		{
	//			const csubstr posStr = transformNode["Scale"].val();

	//			DeserializeVec3f(posStr, &t.Scale);
	//		}

	//		_scene->GetRegistry()->emplace<Transform>(ent, t);
	//	}

	//	if (entityNode.has_child("Renderable"))
	//	{
	//		NodeRef renderableNode = entityNode["Renderable"];
	//		Renderable r{};

	//		if (renderableNode.has_child("MeshKey"))
	//		{
	//			const csubstr posStr = renderableNode["MeshKey"].val();

	//			r.MeshKey = posStr;
	//		}

	//		_scene->GetRegistry()->emplace<Renderable>(ent, r);
	//	}
	//}

	//bool SceneSerializer::DeserializeVec3f(const c4::csubstr& str, glm::vec3* vec)
	//{
	//	uint8_t idx = 0;
	//	const auto splitter = str.split(',');
	//	for (auto it = splitter.begin(); it != splitter.end(); ++it)
	//	{
	//		if (idx >= 3) return false;

	//		std::string valStr = it.m_str.str;
	//		try
	//		{
	//			(*vec)[idx] = stof(valStr);
	//		}
	//		catch (std::invalid_argument ex)
	//		{
	//			return false;
	//		}
	//		idx++;
	//	}

	//	return true;
	//}

	//bool SceneSerializer::DeserializeQuat(const c4::csubstr& str, glm::quat* quat)
	//{
	//	uint8_t idx = 0;
	//	const auto splitter = str.split(',');
	//	for (auto it = splitter.begin(); it != splitter.end(); ++it)
	//	{
	//		if (idx >= 4) return false;

	//		std::string valStr = it.m_str.str;
	//		try
	//		{
	//			(*quat)[idx] = stof(valStr);
	//		}
	//		catch (std::invalid_argument ex)
	//		{
	//			return false;
	//		}
	//		idx++;
	//	}

	//	return true;
	//}

	std::string SceneSerializer::GetFileContents(std::string& filePath)
	{
		std::ifstream file;
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			file.open(filePath);
			std::stringstream fileStream;
			// read file’s buffer contents into streams
			fileStream << file.rdbuf();
			// close file handlers
			file.close();

			return fileStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::FILE::FILE_NOT_SUCCESFULLY_READ" << e.what() << std::endl;
			return "";
		}
	}
}
