#pragma once

struct LaserComponent
{
	float Damage;

	LaserComponent() : Damage(0.0f) {}
	LaserComponent(float damage) : Damage(damage) {}
	LaserComponent(const LaserComponent&) = default;
};