#pragma once
#include "Event.h"
#include "../Core.h"

namespace Mystic
{
	class MYSTIC_API IEventHandler
	{
	public:
		virtual ~IEventHandler() = default;

		virtual void OnEvent(Event& e) = 0;
	};
}
