#pragma once

#include "Scene.h"
#include "../Core/Core.h"

namespace Mystic
{
	class EditorCamera;
	class RuntimeScene;

	class MYSTIC_API ProjectScene : public Scene
	{
	public:
		ProjectScene();
		~ProjectScene() override;

		Ref<RuntimeScene> CreateRuntimeScene();


	public:
		void OnUpdate(EditorCamera& camera);

		friend class SceneSerializer;
	};
}

