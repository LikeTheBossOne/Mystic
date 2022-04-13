#pragma once
#include "../Core.h"
#include <entt/entt.hpp>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

#include "ryml/ryml.hpp"

namespace Mystic
{
	class ProjectScene;

	class MYSTIC_API SceneSerializer
	{
	public:
		SceneSerializer(Ref<ProjectScene> scene);

		void SerializeScene(std::string& filePath);
		void DeserializeScene(std::string& filePath);

	private:
		void SerializeEntity(c4::yml::NodeRef& entNode, uint16_t& strCount, entt::entity entity);

		void SerializeString(std::string& str, c4::yml::NodeRef& node, const char* entChild);


		void DeserializeEntity(const c4::yml::NodeRef& entNode);

		static bool DeserializeVec3f(const c4::csubstr& str, glm::vec3* vec);
		static bool DeserializeQuat(const c4::csubstr& str, glm::quat* vec);


		std::string GetFileContents(std::string& filePath);

	private:
		Ref<ProjectScene> _scene;

		const uint16_t MAX_STRINGS = 1000;
		uint16_t _strCount;
		std::vector<std::string*> _strings{};

	};
}
