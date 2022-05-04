#include "SandboxLayer.h"

#include <iostream>

SandboxLayer::SandboxLayer()
{
	
}

void SandboxLayer::OnUpdate(float deltaTime)
{
	std::cout << "Sandbox tick" << std::endl;
}