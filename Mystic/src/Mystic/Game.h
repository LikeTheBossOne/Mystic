#pragma once
#include <vector>

#include "Core.h"
#include "entt/entt.hpp"

namespace Mystic
{
	class Application;

	class MYSTIC_API Game
	{
	public:
		Game(Ref<Application> app);
		void Start();
		void Update();
		Ref<entt::registry> GetRegistry();

	private:
		Ref<Application> _app;
		Ref<entt::registry> _registry;
	};
}


