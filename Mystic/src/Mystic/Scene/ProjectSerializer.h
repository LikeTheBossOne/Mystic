#pragma once
#include "../Core/Core.h"
#include <entt.hpp>

namespace YAML
{
	class Node;
	class Emitter;
}

namespace Mystic
{
	class ProjectScene;

	class MYSTIC_API ProjectSerializer
	{
	public:
		ProjectSerializer(Ref<ProjectScene> scene);

		void SerializeProject(std::string& filePath);
		bool DeserializeProject(const std::string& filePath);

	private:
		void SerializeAssets(YAML::Emitter& out);
		void SerializeEntity(YAML::Emitter& out, entt::entity entity);

		void DeserializeAssets(YAML::Node& root);

	private:
		Ref<ProjectScene> _scene;

		const uint16_t MAX_STRINGS = 1000;
		uint16_t _strCount;
		std::vector<std::string*> _strings{};
	};
}
