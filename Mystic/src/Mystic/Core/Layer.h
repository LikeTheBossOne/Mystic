#pragma once
#include <string>

#include "Core.h"
#include "Window.h"

namespace Mystic
{
	class Event;

	class MYSTIC_API Layer
	{
	public:
		Layer() {}
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}
	};
}