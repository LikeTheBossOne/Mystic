#pragma once
#include "Camera.h"

namespace Mystic
{
	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera() = default;
		PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip);

		const glm::mat4& GetProjectionMatrix() const { return _projectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return _viewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return _viewProjectionMatrix; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		glm::quat GetOrientation() const;

		const glm::vec3& GetPosition() const { return _position; }
		void SetPosition(const glm::vec3& position) { _position = position; RecalculateViewMatrix(); }

		const glm::vec3& GetRotation() const { return _rotation; }
		void SetRotation(const glm::vec3& rotation) { _rotation = rotation; RecalculateViewMatrix(); }
		
	private:
		void RecalculateViewMatrix();

	private:
		float _FOV = 60.0f;
		float _aspectRatio = 1.778f;
		float _nearClip = 0.1f;
		float _farClip = 1000.0f;
		
		glm::mat4 _projectionMatrix;
		glm::mat4 _viewMatrix;
		glm::mat4 _viewProjectionMatrix;

		glm::vec3 _position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 _rotation = { 0.0f, 0.0f, 0.0f };
	};
}
