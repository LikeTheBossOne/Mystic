#pragma once

#include "Mystic/Scripting/NativeScriptComponent.h"

namespace Mystic
{
	struct Mesh;

	struct Renderable : NativeScriptComponent
	{
		std::string MeshKey;
	};
}
