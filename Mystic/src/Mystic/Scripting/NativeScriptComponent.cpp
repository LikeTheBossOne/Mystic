#include "NativeScriptComponent.h"
#include "Mystic/Scene/Scene.h"

namespace Mystic
{
	NativeScriptComponent::~NativeScriptComponent()
	{
	}

	template <typename T>
	bool NativeScriptComponent::HasComponent() const
	{
		return _owningScene->EntityHasComponent<T>(_ent);
	}

	template <typename T>
	void NativeScriptComponent::RemoveComponent()
	{
		_owningScene->EntityRemoveComponent<T>(_ent);
	}

	template <typename TComponentType>
	TComponentType& NativeScriptComponent::AddComponent()
	{
		return _owningScene->EntityAddNativeScriptComponent<TComponentType>(_ent);
	}

	template <typename T>
	T& NativeScriptComponent::GetComponent() const
	{
		return _owningScene->EntityGetComponent<T>(_ent);
	}
	
}