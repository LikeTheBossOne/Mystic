#include "mystpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Mystic {

	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float verticalFOV, float nearClip, float farClip)
	{
		_projectionType = ProjectionType::Perspective;
		_perspectiveFOV = verticalFOV;
		_perspectiveNear = nearClip;
		_perspectiveFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		_projectionType = ProjectionType::Orthographic;
		_orthographicSize = size;
		_orthographicNear = nearClip;
		_orthographicFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		assert(width > 0 && height > 0);
		_aspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		if (_projectionType == ProjectionType::Perspective)
		{
			_projection = glm::perspective(_perspectiveFOV, _aspectRatio, _perspectiveNear, _perspectiveFar);
		}
		else
		{
			float orthoLeft = -_orthographicSize * _aspectRatio * 0.5f;
			float orthoRight = _orthographicSize * _aspectRatio * 0.5f;
			float orthoBottom = -_orthographicSize * 0.5f;
			float orthoTop = _orthographicSize * 0.5f;

			_projection = glm::ortho(orthoLeft, orthoRight,
				orthoBottom, orthoTop, _orthographicNear, _orthographicFar);
		}

	}

}