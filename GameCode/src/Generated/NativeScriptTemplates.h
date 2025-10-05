#pragma once

#include "Components/AsteroidComponent.h"
#include "Components/LaserComponent.h"

template<> AsteroidComponent& Mystic::NativeScriptComponent::GetComponent<AsteroidComponent>() const
{
	return _owningScene->EntityGetComponent<AsteroidComponent>(_ent);
}

template<> LaserComponent& Mystic::NativeScriptComponent::GetComponent<LaserComponent>() const
{
	return _owningScene->EntityGetComponent<LaserComponent>(_ent);
}

