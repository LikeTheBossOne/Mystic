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

		TransformComponent() : Position(0, 0, 0), Rotation(0, 0, 0, 1), Scale(1, 1, 1) {}
		TransformComponent(glm::vec3 position, glm::quat rotation, glm::vec3 scale) : Position(position), Rotation(rotation), Scale(scale) {}
		TransformComponent(const TransformComponent&) = default;


		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(Rotation);

			return glm::translate(glm::mat4(1.0f), Position)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};
}
