#pragma once
#include "Mystic/Core/MMacros.h"
#include "LaserComponent.generated.h"

MSTRUCT()
struct LaserComponent
{
	MGENERATED_INFO()


	MPROPERTY()
	float Damage;

	LaserComponent() : Damage(0.0f) {}
	LaserComponent(float damage) : Damage(damage) {}
	LaserComponent(const LaserComponent&) = default;
};