#pragma once
#include "../Core.h"

namespace Mystic
{
	enum class MYSTIC_API EventType
	{
		WindowResize = 0,
		MouseButton1_Clicked,
		MouseButton2_Clicked,
		Mouse_Moved,
		Update,
		Render,
		Size,
	};

	class MYSTIC_API Event
	{
	public:
		virtual ~Event() = default;
		virtual EventType GetEventType() const = 0;
	};
}
