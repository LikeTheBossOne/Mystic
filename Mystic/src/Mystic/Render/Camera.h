#pragma once

#include <glm/glm.hpp>
#include "Mystic/Core/Core.h"

namespace Mystic {

	class MYSTIC_API Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection)
			: _projection(projection) {}

		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const { return _projection; }
	protected:
		glm::mat4 _projection = glm::mat4(1.0f);
	};

}