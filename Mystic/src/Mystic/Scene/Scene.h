#pragma once
#include "Mystic/Core/Core.h"
#include "Mystic/ECS/Entity.h"
#include "Mystic/Logging/Log.h"
#include <string>
#include <entt.hpp>

#include "Mystic/Scripting/NativeScriptComponent.h"

namespace Mystic
{
	class AssetLibrary;
	class EditorCamera;
	class Camera;

	class MYSTIC_API Scene
	{
	public:
		Scene();
		~Scene();

		void SetName(const std::string& name);
		[[nodiscard]] std::string GetName() const;

		void ReloadGameCode();
		void SerializeGameCodeEnts();
		void DeserializeGameCodeEnts();
		void GameCodeImGui(entt::entity entity);
		void ClearGameCode();
		void Clear();

		void OnUpdate(float deltaTime);
		void OnEditorUpdate(float deltaTime);
		void OnRender();
		void OnRender(EditorCamera& editorCamera);

		[[nodiscard]] Entity CreateEntity(std::string name);
		[[nodiscard]] Entity CreateEntity(std::string name, GUID guid);
		[[nodiscard]] Entity CreateEntity();
		[[nodiscard]] Entity GetEntity(entt::entity id);

		void DestroyEntity(Entity& entity);

		Ref<Camera> GetMainCamera() { return _mainCamera; }

		template <typename T>
		void RegisterComponentType()
		{
			_registry.storage<T>();
		}

		template <typename T>
		bool EntityHasComponent(entt::entity e)
		{
			return _registry.all_of<T>(e);
		}

		template <typename T>
		void EntityRemoveComponent(entt::entity e)
		{
			Log::Assert(EntityHasComponent<T>(e), "Entity does not have component");
			_registry.remove<T>(e);
		}

		template <typename T, typename... Args>
		T& EntityAddComponent(entt::entity e, Args&&... args)
		{
			Log::Assert(!EntityHasComponent<T>(e), "Entity already has component!");
			T& component = _registry.emplace<T>(e, std::forward<Args>(args)...);
			//OnComponentAdded<Component>(*this, component);
			return component;
		}

		void EntityAddComponentByName(entt::entity e, std::string componentName);

		template <class TComponentType>
		TComponentType& EntityAddNativeScriptComponent(entt::entity e)
		{
			Log::Assert(!EntityHasComponent<TComponentType>(e), "Entity already has component!");
			static_assert(std::is_base_of_v<NativeScriptComponent, TComponentType>, "Attempted to create a non NativeScriptComponent as a component");
			TComponentType& component = _registry.emplace<TComponentType>(e);

			NativeScriptComponent* nativeScriptComp = static_cast<NativeScriptComponent*>(&component());
			nativeScriptComp->_ent = e;
			nativeScriptComp->_owningScene = this;

			return component;
		}

		// Non-ECS Functions
		template <typename T>
		T& EntityGetComponent(entt::entity e)
		{
			Log::Assert(EntityHasComponent<T>(e), "Entity does not have component");
			return _registry.get<T>(e);
		}

		void OnViewportResize(uint32_t width, uint32_t height);

	protected:
		entt::registry _registry;
		Ref<Camera> _mainCamera;
		Ref<AssetLibrary> _assetLibrary;

		std::string _name;

		uint32_t _viewportWidth = 0;
		uint32_t _viewportHeight = 0;

		friend class ProjectSerializer;
		friend class SceneHierarchyPanel;
		friend class EditorLayer;
		friend class GameCodeSystem;
	};
}
