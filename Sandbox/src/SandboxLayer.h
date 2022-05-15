#pragma once

#include "glm/vec4.hpp"
#include "Mystic/Core/Layer.h"
#include "Mystic/Scene/RuntimeScene.h"

class SandboxLayer : public Mystic::Layer
{
public:
	SandboxLayer();
	virtual ~SandboxLayer() = default;

	void OnUpdate(float deltaTime) override;
private:
	Mystic::Ref<Mystic::RuntimeScene> _scene;
};
