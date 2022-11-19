#pragma once
#include "Components/GUIDComponent.h"
#include "entt.hpp"
#include "../Core/Core.h"

namespace Mystic
{
	class MYSTIC_API ComponentRegistry
	{
		static std::vector<std::function<void(entt::registry& dst, entt::registry& src, std::unordered_map<GUID, entt::entity>& uuidToEntMap)>> s_copyFuncs;

	public:
		template<typename Component>
		static void AddComponent() {
			s_copyFuncs.push_back([&](entt::registry& dst, entt::registry& src, std::unordered_map<GUID, entt::entity>& enttMap)
				{
					auto view = src.view<Component>();
					for (auto e : view) {
						GUID guid = src.get<GUIDComponent>(e).GUID;

						entt::entity dstEnttID = enttMap.at(guid);

						// Check if Entity has a component to copy
						if (src.all_of<Component>(e)) {
							auto& component = src.get<Component>(e);
							dst.emplace_or_replace<Component>(dstEnttID, component);
						}
					}
				});
		}

		// Call all functions to copy all components
		static void InvokeCopy(entt::registry& dst, entt::registry& src, std::unordered_map<GUID, entt::entity>& enttMap);
	};
}
