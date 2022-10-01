#pragma once

#include "glm/vec4.hpp"
#include "Mystic/Core/Core.h"
#include "Mystic/Render/Texture.h"

#include "Mystic/Scripting/NativeScriptComponent.h"

namespace Mystic
{
	struct SpriteRendererComponent : NativeScriptComponent
	{
		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}

		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture;
		float TilingFactor = 1.0f;
	};
}
