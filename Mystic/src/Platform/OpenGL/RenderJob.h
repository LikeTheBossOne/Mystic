#pragma once
#include <memory>

#include "Mesh.h"

namespace Mystic
{

	class MYSTIC_API RenderJob
	{
	public:
		RenderJob(std::string& meshKey, glm::mat4& modelMat)
		{
			MeshKey = meshKey;
			ModelMat = std::make_unique<glm::mat4>(modelMat);
		}
		std::string MeshKey;
		Ref<glm::mat4> ModelMat;
	};
}
