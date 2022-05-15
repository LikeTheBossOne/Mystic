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

		Ref<Camera> GetMainCamera() { return _mainCamera; }

		template <typename T>
		bool EntityHasComponent(Entity& e)
		{
			return _registry.all_of<T>(e.EntId);
		}

		template <typename T>
		void EntityRemoveComponent(Entity& e)
		{
			assert(EntityHasComponent<T>(e), "Entity does not have component");
			_registry.remove<T>(e.EntId);
		}

		template <typename T, typename... Args>
		T& EntityAddComponent(Entity& e, Args&&... args)
		{
			assert(!EntityHasComponent<T>(e), "Entity already has component!");
			T& component = _registry.emplace<T>(e.EntId, std::forward<Args>(args)...);
			//OnComponentAdded<Component>(*this, component);
			return component;
		}

		// Non-ECS Functions
		template <typename T>
		T& EntityGetComponent(Entity& e)
		{
			assert(EntityHasComponent<T>(e), "Entity does not have component");
			return _registry.get<T>(e.EntId);
		}

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
