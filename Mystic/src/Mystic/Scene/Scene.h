#pragma once
#include "../Core.h"
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

		[[nodiscard]] Entity CreateEntity(std::string& name) const;
		[[nodiscard]] Entity CreateEntity() const;

		[[nodiscard]] Ref<entt::registry> GetRegistry() const;

		virtual void RenderScene() = 0;

	protected:
		Ref<entt::registry> _registry;
		Ref<Camera> _activeCamera;

		std::string _name;
	};
}
