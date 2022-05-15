#pragma once

namespace Mystic
{
	struct CharacterComponent
	{
		bool Active;

		CharacterComponent() : Active(false) {}
		CharacterComponent(bool active) : Active(active) {}
		CharacterComponent(const CharacterComponent&) = default;
	};
}