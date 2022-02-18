#pragma once

#include <memory>
#include <unordered_map>

#include "../Core.h"
#include "Event.h"

namespace Mystic
{
	class IEventHandler;

	class MYSTIC_API EventSystem
	{
	public:
		static void Init();

		static void BindHandler(std::weak_ptr<IEventHandler> handler, EventType type);
		static void RaiseEvent(Event& e);

	private:
		static std::unordered_map<EventType, std::vector<std::weak_ptr<IEventHandler>>> s_handlerMap;
	};
}
