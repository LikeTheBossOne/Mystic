#include "mystpch.h"
#include "Window.h"

#ifdef MYST_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Mystic
{
	Unique<Window> Window::Create(const WindowProps& props)
	{
#ifdef MYST_PLATFORM_WINDOWS
		return std::make_unique<WindowsWindow>(props);
#else
		assert(false, "Unknown platform!");
		return nullptr;
#endif
	}

}