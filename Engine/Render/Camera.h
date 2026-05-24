//
// Created by sunvy on 15/12/2025.
//

#ifndef SUNSETCRAFT_CAMERA_H
#define SUNSETCRAFT_CAMERA_H

namespace SunsetEngine
{
    struct AABB;

    struct Frustum
    {
        glm::vec4 planes[6]; // left, right, bottom, top, near, far

        bool IsVisible(const AABB& box);
    };

    class Camera
    {
    public:
        Camera();
        ~Camera();

        glm::mat4 GetProjection() const;
        glm::mat4 GetViewMatrix() const;

        glm::vec3 GetPosition() const;
        glm::vec3 GetForward() const;
        glm::vec3 GetUp() const;

        void MoveForward(float speed);

        void MoveBackward(float speed);

        void MoveRight(float speed);

        void MoveLeft(float speed);

        void MoveUp(float speed);

        void SetForward(const glm::vec3& forward);

        void AddPosition(const glm::vec3& position);

        void SetPosition(const glm::vec3& position);

        void AddPitch(float pitch);

        void AddYaw(float yaw);

        Frustum GetFrustum() const;

    private:
        glm::vec3 m_Position;
        glm::vec3 m_Forward;
        glm::vec3 m_Up;

        float m_Yaw, m_Pitch;

        float fov;
    };
}

#endif //SUNSETCRAFT_CAMERA_H
