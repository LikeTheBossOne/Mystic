#pragma once
#include "Event.h"

namespace Mystic
{
	class IEventHandler
	{
		public:
		virtual ~IEventHandler() = 0;
	
		virtual void OnEvent(Event& e) = 0;
	};
}
