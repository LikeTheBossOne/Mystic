#include "Entity.h"

#include "Components/GUIDComponent.h"

namespace Mystic
{
	GUID Entity::GetGUID() const
	{
		GUIDComponent comp =  _registry->get<GUIDComponent>(EntId);
		return comp.GUID;
	}
}
