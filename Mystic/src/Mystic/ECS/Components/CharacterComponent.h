#pragma once

#include "Mystic/Scripting/NativeScriptComponent.h"

namespace Mystic
{
	struct CharacterComponent : NativeScriptComponent
	{
		bool Active;

		CharacterComponent() : Active(false) {}
		CharacterComponent(bool active) : Active(active) {}
		CharacterComponent(const CharacterComponent&) = default;
	};
}