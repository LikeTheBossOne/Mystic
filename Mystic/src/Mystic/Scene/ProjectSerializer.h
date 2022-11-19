#pragma once
#include "../Core/Core.h"
#include <entt.hpp>

namespace YAML
{
	namespace detail
	{
		struct iterator_value;
	}

	class Node;
	class Emitter;
}

namespace Mystic
{
	class Scene;

	class MYSTIC_API ProjectSerializer
	{
	public:
		ProjectSerializer(Ref<Scene> scene);

		void SerializeProject(std::string& filePath);
		bool DeserializeProject(const std::string& filePath);

		void SerializeGameCodeEnts(std::string& filePath);
		bool DeserializeGameCodeEnts(std::string& filePath);

	private:
		void SerializeAssets(YAML::Emitter& out);
		void SerializeEntity(YAML::Emitter& out, entt::entity entity);

		void DeserializeAssets(YAML::Node& root);
		void DeserializeEntity(YAML::detail::iterator_value& entityNode);

	private:
		Ref<Scene> _scene;

		const uint16_t MAX_STRINGS = 1000;
		uint16_t _strCount;
		std::vector<std::string*> _strings{};
	};
}
