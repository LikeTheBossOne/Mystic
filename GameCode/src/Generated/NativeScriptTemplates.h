#pragma once

#include "Mystic/Scripting/NativeScriptComponent.h"
#include "Components/LaserComponent.h"
#include "Mystic/Scene/Scene.h"

template<> LaserComponent& Mystic::NativeScriptComponent::GetComponent<LaserComponent>() const
{
	return _owningScene->EntityGetComponent<LaserComponent>(_ent);
}