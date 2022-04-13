#pragma once

#include "Scene.h"
#include "../Core.h"

#include "../ECS/Components/Renderable.h"
#include "../ECS/Components/Transform.h"

namespace Mystic
{
	struct Renderable;
	struct Transform;

	class MYSTIC_API RuntimeScene : public Scene
	{
	public:
		RuntimeScene();
		~RuntimeScene() override;

		void RenderScene() override;


		friend class ProjectScene;

	};
}


