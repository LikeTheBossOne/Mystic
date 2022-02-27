#pragma once
#include <vector>

#include "Core.h"
#include "entt/entt.hpp"

namespace Mystic
{
	class Scene;
	class Application;

	class MYSTIC_API Game
	{
	public:
		Game(Ref<Application> app);
		void Start();
		void Update();
		Ref<Scene> GetScene();

	private:
		Ref<Application> _app;
		
		Ref<Scene> _currentScene;
	};
}


