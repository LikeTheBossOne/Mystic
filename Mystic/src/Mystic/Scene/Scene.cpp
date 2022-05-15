#include "Scene.h"
#include "../ECS/Components/GUIDComponent.h"
#include "../ECS/Components/TagComponent.h"
#include "Mystic/ECS/Components/CameraComponent.h"
#include "Mystic/ECS/Components/TransformComponent.h"

namespace Mystic
{
	Scene::~Scene()
	{
	}

	void Scene::SetName(const std::string& name)
	{
		_name = name;
	}

	std::string Scene::GetName() const
	{
		return _name;
	}

	Entity Scene::CreateEntity()
	{
		return CreateEntity("default");
	}

	Entity Scene::GetEntity(entt::entity id)
	{
		Entity entity{};
		if (!_registry.valid(id))
		{
			return entity;
		}

		GUIDComponent comp = _registry.get<GUIDComponent>(id);
		entity.EntId = id;
		entity.GUID = comp.GUID;
		entity.OwningScene = this;

		return entity;
	}

	void Scene::DestroyEntity(Entity& entity)
	{
		_registry.destroy(entity.EntId);
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		_viewportWidth = width;
		_viewportHeight = height;

		// Resize our non-FixedAspectRatio cameras
		auto view = _registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}

	}

	Entity Scene::CreateEntity(std::string name)
	{
		GUID entGuid;
		HRESULT result = CoCreateGuid(&entGuid);
		assert(result == S_OK, "GUID Creation failed");
		
		return CreateEntity(name, entGuid);
	}

	Entity Scene::CreateEntity(std::string name, GUID guid)
	{
		Entity ent(_registry.create(), guid, this);

		_registry.emplace<GUIDComponent>(ent.EntId, GUIDComponent{ guid });
		_registry.emplace<TagComponent>(ent.EntId, TagComponent{ name });
		_registry.emplace<TransformComponent>(ent.EntId);

		return ent;
	}
}
