#pragma once
#include "glm/vec3.hpp"
#include "Mystic/Scripting/NativeScriptComponent.h"

namespace Mystic
{
	struct VelocityComponent : NativeScriptComponent
	{
		glm::vec3 Velocity;

		VelocityComponent() : Velocity(0, 0, 0) {}
		VelocityComponent(glm::vec3 vel) : Velocity(vel) {}
		VelocityComponent(const VelocityComponent&) = default;

	};
}
