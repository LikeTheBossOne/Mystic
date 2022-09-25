#include "mystpch.h"
#include "Mystic/Core/Input.h"

#include "Mystic/Core/Application.h"
#include "Mystic/Core/Window.h"
#include <GLFW/glfw3.h>

#include "glm/vec2.hpp"
#include "Mystic/Core/ServiceLocator.h"
#include "singleton.h"

namespace Mystic {

	bool Input::IsKeyPressed(const KeyCode key)
	{
		ServiceLocator& locator = singleton<ServiceLocator>();
		auto* window = static_cast<GLFWwindow*>(locator.GetApplication().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, static_cast<int32_t>(key));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(const MouseCode button)
	{
		ServiceLocator& locator = singleton<ServiceLocator>();
		auto* window = static_cast<GLFWwindow*>(locator.GetApplication().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
		return state == GLFW_PRESS;
	}

	glm::vec2 Input::GetMousePosition()
	{
		ServiceLocator& locator = singleton<ServiceLocator>();
		auto* window = static_cast<GLFWwindow*>(locator.GetApplication().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	float Input::GetMouseX()
	{
		return GetMousePosition().x;
	}

	float Input::GetMouseY()
	{
		return GetMousePosition().y;
	}

}
