#pragma once


template<> LaserComponent& Mystic::NativeScriptComponent::GetComponent<LaserComponent>() const
{
	return _owningScene->EntityGetComponent<LaserComponent>(_ent);
}

