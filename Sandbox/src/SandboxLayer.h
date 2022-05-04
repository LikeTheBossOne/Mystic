#pragma once

#include "glm/vec4.hpp"
#include "Mystic/Core/Layer.h"

class SandboxLayer : public Mystic::Layer
{
public:
	SandboxLayer();
	virtual ~SandboxLayer() = default;

	void OnUpdate(float deltaTime) override;
private:
	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};