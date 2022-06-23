#pragma once
#include <string>

#include "Mystic/Core/Core.h"

namespace Mystic
{
	class Material
	{

	public:
		Ref<Material> CreateMaterialFromFBX(std::string assetPath); //TODO: Pre-load meshes and materials into engine format in a single FBX file-access 
	};
}
