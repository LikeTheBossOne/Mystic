#include "ComponentRegistry.h"

namespace Mystic
{
	std::vector<std::function<void(entt::registry& dst, entt::registry& src, std::unordered_map<GUID, entt::entity>& enttMap)>> ComponentRegistry::s_copyFuncs;

	// Call all functions to copy all components
	void ComponentRegistry::InvokeCopy(entt::registry& dst, entt::registry& src, std::unordered_map<GUID, entt::entity>& enttMap) {
		for (auto& func : s_copyFuncs) {
			func(dst, src, enttMap);
		}
	}
}