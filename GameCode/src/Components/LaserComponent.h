#pragma once
#include "Mystic/Core/MMacros.h"
#include "Mystic/Scripting/NativeScriptComponent.h"
#include "LaserComponent.generated.h"

MCOMPONENT()
class LaserComponent : Mystic::NativeScriptComponent
{
	MGENERATED_INFO()

public:
	MPROPERTY()
	float Damage = 0.0f;

	MPROPERTY()
	int TestInt = 10;

	void Start();
	void Update();

	LaserComponent(const LaserComponent&) = default;
	
};