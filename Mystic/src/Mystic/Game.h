#pragma once
#include <vector>

#include "Core.h"
#include "entt.hpp"

namespace Mystic
{
	class RuntimeScene;
	class Application;

	class MYSTIC_API Game
	{
	public:
		Game(Ref<Application> app, Ref<RuntimeScene> startingScene);

		void Start();
		void Update();
		Ref<RuntimeScene> GetScene();

	private:
		Ref<Application> _app;
		
		Ref<RuntimeScene> _currentScene;
	};
}


