#pragma once

#include "Scene.h"
#include "../Core.h"

#include "../ECS/Components/Renderable.h"
#include "../ECS/Components/Transform.h"

namespace Mystic
{
	class RuntimeScene;

	class MYSTIC_API ProjectScene : public Scene
	{
	public:
		ProjectScene();
		~ProjectScene() override;

		Ref<RuntimeScene> CreateRuntimeScene();


	public:
		void RenderScene() override;

	};
}

