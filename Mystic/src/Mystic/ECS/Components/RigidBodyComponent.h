#pragma once
#include "glm/vec3.hpp"

#include "Mystic/Scripting/NativeScriptComponent.h"

namespace Mystic
{
	struct RigidBodyComponent : NativeScriptComponent
	{
		glm::vec3 Acceleration;

		RigidBodyComponent() : Acceleration(0, 0, 0) {}
		RigidBodyComponent(glm::vec3 acceleration) : Acceleration(acceleration) {}
		RigidBodyComponent(const RigidBodyComponent&) = default;
	};
}
