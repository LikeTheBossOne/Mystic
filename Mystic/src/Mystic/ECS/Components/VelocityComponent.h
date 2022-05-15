#pragma once
#include "glm/vec3.hpp"

namespace Mystic
{
	struct VelocityComponent
	{
		glm::vec3 Velocity;

		VelocityComponent() : Velocity(0, 0, 0) {}
		VelocityComponent(glm::vec3 vel) : Velocity(vel) {}
		VelocityComponent(const VelocityComponent&) = default;

	};
}
