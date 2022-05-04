#pragma once
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Mystic
{
	struct TransformComponent
	{

		glm::vec3 Position;
		glm::quat Rotation;
		glm::vec3 Scale;

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(Rotation);

			return glm::translate(glm::mat4(1.0f), Position)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};
}
