#pragma once
#include "Mystic/ECS/Components/TransformComponent.h"
#include "Mystic/Scene/Scene.h"

namespace Mystic
{

	//template __declspec(dllexport) TransformComponent& Scene::EntityGetComponent<TransformComponent>(Entity& e);
	/*template TransformComponent& Scene::EntityGetComponent<TransformComponent>(Entity& e);
	template TransformComponent& Scene::EntityGetComponent<TransformComponent>(Entity& e);
	template TransformComponent& Scene::EntityGetComponent<TransformComponent>(Entity& e);
	template TransformComponent& Scene::EntityGetComponent<TransformComponent>(Entity& e);
	template TransformComponent& Scene::EntityGetComponent<TransformComponent>(Entity& e);
	template TransformComponent& Scene::EntityGetComponent<TransformComponent>(Entity& e);
	template TransformComponent& Scene::EntityGetComponent<TransformComponent>(Entity& e);
	template TransformComponent& Scene::EntityGetComponent<TransformComponent>(Entity& e);
	template TransformComponent& Scene::EntityGetComponent<TransformComponent>(Entity& e);*/

	template<> TransformComponent& NativeScriptComponent::GetComponent<TransformComponent>() const
	{
		return _owningScene->EntityGetComponent<TransformComponent>(_ent);
	}
}


