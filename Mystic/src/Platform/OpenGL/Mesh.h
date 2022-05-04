#pragma once
#include "../../Mystic/Core/Core.h"

namespace Mystic
{
	struct MYSTIC_API Mesh
	{
		Mesh(std::string& key)
		{
			Key = key;
			VAO = 0;
			VBO = 0;
		}

		std::string Key;
		uint32_t VAO;
		uint32_t VBO;
	};
}
