#include "PerspectiveCamera.h"

#include "glm/ext/matrix_clip_space.hpp"

namespace Mystic
{
	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: _FOV(fov), _aspectRatio(aspectRatio), _nearClip(nearClip), _farClip(farClip),
		_projectionMatrix(glm::perspective(fov, aspectRatio, nearClip, farClip))
	{

	}
}
