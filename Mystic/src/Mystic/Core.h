#pragma once

#ifdef MYST_PLATFORM_WINDOWS
	#ifdef MYST_BUILD_DLL
		#define MYSTIC_API __declspec(dllexport)
	#else
		#define MYSTIC_API __declspec(dllimport)
	#endif
#else
	#error Mystic is Windows-only
#endif