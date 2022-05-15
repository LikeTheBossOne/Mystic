#pragma once
#include <functional>

#include "System.h"

namespace Mystic
{
	class SystemRegistry
	{
	public:
		static std::unordered_map<std::string, std::pair<Ref<System>, bool>> s_systems;

		template<class Sys>
		static void AddSystem(std::string sysName)
		{
			Ref<System> s = std::make_shared<Sys>();
			std::pair pair = { s, false };
			s_systems.try_emplace(sysName, pair);
		}
		
		static void EnableSystem(std::string sysName)
		{
			//TODO: Add logging for bad sys names
			if (s_systems.contains(sysName))
			{
				s_systems[sysName].second = true;
			}
		}
		
		static void DisableSystem(std::string sysName)
		{
			//TODO: Add logging for bad sys names
			if (s_systems.contains(sysName))
			{
				s_systems[sysName].second = false;
			}
		}
	};
}
