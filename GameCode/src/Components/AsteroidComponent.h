#pragma once
#include <cstdint>

struct AsteroidComponent
{
	uint8_t SplitsLeft;

	AsteroidComponent() : SplitsLeft(2) {}
	AsteroidComponent(uint8_t splitsLeft) : SplitsLeft(splitsLeft) {}
	AsteroidComponent(const AsteroidComponent&) = default;
};

