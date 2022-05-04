#pragma once
#include "../Core/Core.h"
#include "../ECS/Entity.h"
#include <string>
#include <entt.hpp>

namespace Mystic
{
	class Camera;

	class MYSTIC_API Scene
	{
	public:
		virtual ~Scene();

		void SetName(const std::string& name);
		[[nodiscard]] std::string GetName() const;

		[[nodiscard]] Entity CreateEntity(std::string name);
		[[nodiscard]] Entity CreateEntity(std::string name, GUID guid);
		[[nodiscard]] Entity CreateEntity();
		[[nodiscard]] Entity GetEntity(entt::entity id);

		void DestroyEntity(Entity& entity);

		void OnViewportResize(uint32_t width, uint32_t height);

	protected:
		entt::registry _registry;
		Ref<Camera> _mainCamera;

		std::string _name;

		uint32_t _viewportWidth = 0;
		uint32_t _viewportHeight = 0;

		friend class SceneHierarchyPanel;
		friend class EditorLayer;
	};
}
