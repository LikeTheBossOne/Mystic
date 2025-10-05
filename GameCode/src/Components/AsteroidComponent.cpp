#include "AsteroidComponent.h"

#include "Mystic/ECS/Components/TransformComponent.h"
#include "Mystic/Scripting/Utilities/Random.h"

void AsteroidComponent::Start()
{
}

void AsteroidComponent::Update(float dt)
{
	auto& transform = GetComponent<Mystic::TransformComponent>();
	float dx = Mystic::Random::Int(30, 100);
	float dy = Mystic::Random::Int(30, 100);
	transform.Position.x += dx * dt;
	transform.Position.y += dy * dy;
}