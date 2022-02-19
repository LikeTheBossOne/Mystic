#pragma once

#include "Event.h"
#include "../Core.h"

namespace Mystic
{
	class MYSTIC_API RenderEvent : public Event
	{
	public:
		RenderEvent() = default;
		EventType GetEventType() const override { return EventType::Render; }
	};
}