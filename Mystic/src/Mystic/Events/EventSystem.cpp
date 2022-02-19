#include "EventSystem.h"

#include "IEventHandler.h"

namespace Mystic
{
	std::unordered_map<EventType, std::vector<std::weak_ptr<IEventHandler>>> EventSystem::s_handlerMap{};
	
	void EventSystem::Init()
	{
		for (int i = 0; i < static_cast<int>(EventType::Size); i++)
		{
			s_handlerMap[static_cast<EventType>(i)] = std::vector<std::weak_ptr<IEventHandler>>();
		}
	}

	void EventSystem::BindHandler(std::weak_ptr<IEventHandler> handler, EventType type)
	{
		s_handlerMap[type].emplace_back(handler);
	}

	void EventSystem::RaiseEvent(Event& e)
	{
		EventType eventType = e.GetEventType();
		for (auto iter = s_handlerMap[eventType].begin(); iter < s_handlerMap[eventType].end() ;)
		{
			if (Ref<IEventHandler> shrdPtr = (*iter).lock())
			{
				shrdPtr->OnEvent(e);
				++iter;
			}
			else
			{
				s_handlerMap[eventType].erase(iter);
			}
		}
	}
}
