#pragma once
#include "Core.h"
#include "Events/IEventHandler.h"

namespace Mystic
{
	class MYSTIC_API Layer : public Mystic::IEventHandler
	{
	public:
		virtual ~Layer() = default;

		virtual void OnEvent(Event& e) = 0;
	};
}
