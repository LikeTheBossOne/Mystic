#pragma once
#include "Mystic/Scene/SceneCamera.h"
#include "Mystic/Scripting/NativeScriptComponent.h"

namespace Mystic
{
	struct CameraComponent : NativeScriptComponent
	{
		SceneCamera Camera;
		bool Primary = true; // TODO: think about moving to Scene
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};
}
