#pragma once
#include <cstdint>

#include "Mystic/Core/MMacros.h"
#include "Mystic/Scripting/NativeScriptComponent.h"
#include "AsteroidComponent.generated.h"

MCOMPONENT()
class AsteroidComponent : Mystic::NativeScriptComponent
{
	MGENERATED_INFO()

public:
	MPROPERTY()
	uint8_t SplitsLeft;

	void Start();

	void Update(float dt);


	AsteroidComponent(const AsteroidComponent&) = default;
};

