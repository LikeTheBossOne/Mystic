#pragma once

#include "glm/vec2.hpp"

#include "Mystic/Core/KeyCodes.h"
#include "Mystic/Core/MouseCodes.h"

namespace Mystic {

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
		
	private:
	};
}
