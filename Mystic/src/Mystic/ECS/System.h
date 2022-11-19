#pragma once
#include "Mystic/Core/Core.h"
#include "entt.hpp"

namespace Mystic
{
	class Scene;

	class System
	{
	public:
		System() = default;
		virtual ~System() = default;

		virtual void OnUpdate(Scene* scene, entt::registry& registry, float deltaTime) {}
	protected:
		bool _useSystem = false;
	};
}
