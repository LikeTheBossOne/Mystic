#pragma once
#include <string>

#include "glm/vec4.hpp"
#include "Mystic/Render/Mesh.h"
#include "Mystic/Render/Texture.h"

namespace Mystic
{
	struct MeshRendererComponent
	{
		std::string MeshName;

		std::string ShaderName;

		//TODO: Combine Texture, color, and shader into a single 'Material'
		std::string TextureName;
		glm::vec4 Color;
	};
}

