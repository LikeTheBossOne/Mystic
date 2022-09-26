#pragma once
#include "Mystic/Core/MMacros.h"

MSTRUCT()
struct LaserComponent
{
	MPROPERTY()
	float Damage;

	void Update();

	LaserComponent() : Damage(0.0f) {}
	LaserComponent(float damage) : Damage(damage) {}
	LaserComponent(const LaserComponent&) = default;
};