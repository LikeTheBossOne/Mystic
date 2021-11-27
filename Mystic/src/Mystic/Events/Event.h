#pragma once

namespace Mystic
{
	enum class EventType
	{
		WindowResize = 0, MouseButton1_Clicked, MouseButton2_Clicked, Mouse_Moved,
		Size,
	};

	class Event
	{
		public:
		virtual EventType GetEventType() const = 0;
	};
}
