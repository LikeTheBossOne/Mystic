#include "LaserComponent.h"

#include <iostream>

#include "Mystic/ECS/Components/TransformComponent.h"
#include "Mystic/Core/exportimport.h"
#include "Generated/NativeScriptTemplates.h"

void LaserComponent::Start()
{
}

void LaserComponent::Update()
{
	auto& position = GetComponent<Mystic::TransformComponent>();
	auto& me = GetComponent<LaserComponent>();
	std::cout << "GameCode::LaserComponent::Update" << std::endl;
}
