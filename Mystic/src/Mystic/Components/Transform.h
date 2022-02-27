#pragma once
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Mystic
{
	struct Transform
	{

		glm::vec3 Position;
		glm::quat Rotation;
		glm::vec3 Scale;
	};
}
