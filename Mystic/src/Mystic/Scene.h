#pragma once
#include <entt/entt.hpp>

#include "Core.h"

namespace Mystic
{
	class Camera;

	class MYSTIC_API Scene
	{
	public:
		Scene();

		void RenderScene();

		Ref<entt::registry> GetRegistry();

	private:
		Ref<entt::registry> _registry;
		Ref<Camera> _activeCamera;
	};
}

