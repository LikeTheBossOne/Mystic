#pragma once
#include <memory>

#ifdef MYST_PLATFORM_WINDOWS
	#ifdef MYST_BUILD_DLL
		#define MYSTIC_API __declspec(dllexport)
	#else
		#define MYSTIC_API __declspec(dllimport)
	#endif
#else
	#error Mystic is Windows-only
#endif


#define BIT(x) (1 << x)

#define MYST_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#ifdef MYST_DISABLE_DUMB_WARNINGS
	#pragma warning(disable: 4251)
	
#endif

namespace Mystic
{
	template<typename T>
	using Unique = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}