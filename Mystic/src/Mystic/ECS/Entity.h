#pragma once

#include "../Core.h"
#include "entt/entt.hpp"
#pragma comment(lib, "rpcrt4.lib")  // UuidCreate - Minimum supported OS Win 2000
#include <windows.h>

namespace Mystic
{
	struct GUIDComponent;

	class MYSTIC_API Entity
	{
	public:
		Entity(entt::entity id, Ref<entt::registry> registry) : EntId(id), _registry(registry)
		{
			
		}

	public:
		entt::entity EntId;

		GUID GetGUID() const;

	private:
		Ref<entt::registry> _registry;
	};
}