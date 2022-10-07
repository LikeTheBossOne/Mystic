#pragma once
#include "Mystic/Core/Core.h"
#include "entt.hpp"

namespace Mystic
{
	class Scene;
	class Entity;

	class MYSTIC_API NativeScriptComponent
	{
	public:
		//virtual void Init() = 0;
		//virtual void Start() = 0;
		//virtual void Update() = 0;

		~NativeScriptComponent();

		template <typename T>
		bool HasComponent() const;

		template <typename T>
		void RemoveComponent();

		template <typename TComponentType>
		TComponentType& AddComponent();

		template <typename T>
		T& GetComponent() const;

	protected:
		entt::entity _ent = entt::null;
		Scene* _owningScene;

		friend class Scene;
	};
}