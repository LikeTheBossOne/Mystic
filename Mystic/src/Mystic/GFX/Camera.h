#pragma once
#include <glm/mat4x2.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace Mystic
{
    class Camera
    {
    public:
        static constexpr float DEFAULT_YAW = -90.f;
        static constexpr float DEFAULT_SENSITIVITY = 0.1f;
        static constexpr float DEFAULT_FOV = 45.0f;
        static constexpr float DEFAULT_PITCH_ = 0.0f;

        glm::vec3 _position;
        glm::vec3 _front;
        glm::vec3 _up;
        glm::vec3 _right;
        glm::vec3 _worldUp;

        float _yaw;
        float _pitch;
        float _mouseSensitivity;
        float _fov;

        Camera(glm::vec3 position = glm::vec3(0.f, 0.f, 0.f), glm::vec3 up = glm::vec3(0.f, 1.f, 0.f),
            float yaw = DEFAULT_YAW, float pitch = DEFAULT_PITCH_);

        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

        glm::mat4 GetViewMatrix();

        void UpdatePos(glm::vec3 newPos);

        void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch);

        void ProcessMouseScroll(float yOffset);

    private:
        void UpdateCameraVectors();
    };

}
