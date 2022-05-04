#pragma once

#include "../Core/Core.h"
#include "entt.hpp"
#pragma comment(lib, "rpcrt4.lib")  // UuidCreate - Minimum supported OS Win 2000
#include <windows.h>

namespace Mystic
{
	class Scene;
	struct GUIDComponent;
	
	class MYSTIC_API Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity id, GUID guid, Scene* owningScene) : EntId(id), GUID(guid), OwningScene(owningScene)
		{
			
		}

		operator bool() const { return EntId != entt::null; }

	public:
		entt::entity EntId;

		GUID GUID;

		Scene* OwningScene;
	};
}