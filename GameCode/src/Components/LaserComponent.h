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
	float Damage = 10.0f;

	MPROPERTY()
	float Range = 5.0f;

	MPROPERTY()
	int TestInt = 12;

	void Start();
	void Update();

	LaserComponent(const LaserComponent&) = default;
	
};