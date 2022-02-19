#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Core.h"

namespace Mystic
{
	class Game;

	class MYSTIC_API Application
	{
	public:
		Application();
		virtual ~Application() = default;

		virtual void Start(std::shared_ptr<Game> game);
		virtual void Update() = 0;
		virtual void Render() = 0;

		void Close();
		bool ShouldClose();

	private:
		bool _shouldClose;
		std::shared_ptr<Game> Game;
	};

	// To be defined in client
	Application* CreateApplication();
}

