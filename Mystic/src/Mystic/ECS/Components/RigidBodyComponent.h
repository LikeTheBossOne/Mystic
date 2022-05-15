#pragma once
#include "glm/vec3.hpp"

namespace Mystic
{
	struct RigidBodyComponent
	{
		glm::vec3 Acceleration;

		RigidBodyComponent() : Acceleration(0, 0, 0) {}
		RigidBodyComponent(glm::vec3 acceleration) : Acceleration(acceleration) {}
		RigidBodyComponent(const RigidBodyComponent&) = default;
	};
}
