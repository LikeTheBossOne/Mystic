﻿#pragma once
#include "Mystic/Scripting/NativeScriptComponent.h"

namespace Mystic
{
	struct ThrusterComponent : NativeScriptComponent
	{
		float Thrust;
		// Rads/s
		float RotationRate;

		ThrusterComponent() : Thrust(5.f), RotationRate(10.f) {}
		ThrusterComponent(float thrust, float rotationRate) : Thrust(thrust), RotationRate(rotationRate) {}
		ThrusterComponent(const ThrusterComponent&) = default;
	};
}
