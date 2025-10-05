#include "Random.h"

#include <chrono>
#include <random>

std::default_random_engine _rand = std::default_random_engine(
	std::chrono::system_clock::now().time_since_epoch().count()
);

int Mystic::Random::Int(int min, int max)
{
	std::uniform_int_distribution distribution(min, max);

	return distribution(_rand);
}
