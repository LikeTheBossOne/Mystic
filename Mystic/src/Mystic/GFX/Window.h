#pragma once
#include "../Core.h"

namespace Mystic
{
	class MYSTIC_API Window
	{
	public:
		Window() = default;
		virtual ~Window() = default;

		virtual void OpenWindow(int width, int height) = 0;
		virtual void Clear(float r, float g, float b, float a) = 0;
		virtual void SwapBuffer() = 0;
		virtual bool ShouldClose() = 0;
		virtual void PollEvents() = 0;
	};
}
