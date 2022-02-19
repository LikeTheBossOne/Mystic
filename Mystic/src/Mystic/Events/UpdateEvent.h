#pragma once

#include "Event.h"
#include "../Core.h"

namespace Mystic
{
	class MYSTIC_API UpdateEvent : public Event
	{
	public:
		UpdateEvent() = default;
		EventType GetEventType() const override { return EventType::Update; }
	};
}