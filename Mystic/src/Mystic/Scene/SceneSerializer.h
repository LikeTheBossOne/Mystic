#pragma once
#include "../Core/Core.h"
#include <entt.hpp>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

namespace YAML
{
	class Emitter;
}

namespace Mystic
{
	class ProjectScene;

	class MYSTIC_API SceneSerializer
	{
	public:
		SceneSerializer(Ref<ProjectScene> scene);

		void SerializeScene(std::string& filePath);
		bool DeserializeScene(const std::string& filePath);

	private:
		void SerializeEntity(YAML::Emitter& out, entt::entity entity);

	private:
		Ref<ProjectScene> _scene;

		const uint16_t MAX_STRINGS = 1000;
		uint16_t _strCount;
		std::vector<std::string*> _strings{};
	};
}
