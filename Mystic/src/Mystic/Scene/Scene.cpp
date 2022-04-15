#include "Scene.h"
#include "../ECS/Components/GUIDComponent.h"
#include "../ECS/Components/InEditorNameComponent.h"

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

	Entity Scene::CreateEntity() const
	{
		std::string defaultName("entity");
		return CreateEntity(defaultName);
	}

	Entity Scene::CreateEntity(std::string& name) const
	{
		Entity ent(_registry->create(), _registry);

		GUID entGuid;
		HRESULT result = CoCreateGuid(&entGuid);
		assert(result == S_OK, "GUID Creation failed");

		_registry->emplace<GUIDComponent>(ent.EntId, GUIDComponent{ entGuid });
		_registry->emplace<InEditorNameComponent>(ent.EntId, InEditorNameComponent{ name });

		return ent;
	}

	Ref<entt::registry> Scene::GetRegistry() const
	{
		return _registry;
	}
}
