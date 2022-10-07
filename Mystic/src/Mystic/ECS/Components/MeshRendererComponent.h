#pragma once
#include <string>

#include "glm/vec4.hpp"
#include "Mystic/Scripting/NativeScriptComponent.h"


namespace Mystic
{
	struct MeshRendererComponent : NativeScriptComponent
	{
		std::string MeshName;

		std::string ShaderName;

		//TODO: Combine Texture, color, and shader into a single 'Material'
		std::string TextureName;
		glm::vec4 Color;
	};
}

