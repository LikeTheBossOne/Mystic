#pragma once

#include <string>
#include <vector>

#include "Core.h"

namespace Mystic
{

	class MYSTIC_API Application
	{
	public:
		Application() {}
		virtual ~Application() {}

		virtual void Start();
		virtual void Update();
		virtual void Render();
	};

	// To be defined in client
	Application* CreateApplication();
}

