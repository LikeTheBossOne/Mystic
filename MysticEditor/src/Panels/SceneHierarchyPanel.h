#pragma once
#include "Mystic/Scene/Scene.h"

namespace Mystic
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>&scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();

		Entity GetSelectedEntity() const { return _selectionContext; }
		void SetSelectedEntity(Entity entity);

	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

	private:
		Ref<Scene> _sceneContext;
		Entity _selectionContext;
	};
}
