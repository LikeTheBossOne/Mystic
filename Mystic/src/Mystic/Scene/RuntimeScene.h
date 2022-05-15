#pragma once

#include "Scene.h"
#include "../Core/Core.h"

namespace Mystic
{
	struct Renderable;
	struct TransformComponent;

	class MYSTIC_API RuntimeScene : public Scene
	{
	public:
		RuntimeScene();
		~RuntimeScene() override;

		void OnUpdate(float deltaTime);


		friend class ProjectScene;

	};
}


