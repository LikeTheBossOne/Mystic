#pragma once

#include "Mystic/Scripting/NativeScriptComponent.h"

namespace Mystic
{
	struct TagComponent : NativeScriptComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string & tag)
			: Tag(tag) {}
	};
}