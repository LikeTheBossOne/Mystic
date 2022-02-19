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
		Game(std::shared_ptr<Application> app);
		void Start();
		void Update();
		std::shared_ptr<entt::registry> GetRegistry();

	private:
		std::shared_ptr<Application> _app;
		std::shared_ptr<entt::registry> _registry;
	};
}


