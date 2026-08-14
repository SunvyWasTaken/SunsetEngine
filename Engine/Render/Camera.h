//
// Created by sunvy on 15/12/2025.
//

#ifndef SUNSETCRAFT_CAMERA_H
#define SUNSETCRAFT_CAMERA_H

namespace Sunset
{
    class BinaryInputArchive;
    class BinaryOutputArchive;
    struct AABB;
    class Camera;

    void Serialize(BinaryInputArchive& archive, Camera& camera);
    void Serialize(BinaryOutputArchive& archive, Camera& camera);

    struct Frustum
    {
        glm::vec4 planes[6]; // left, right, bottom, top, near, far

        bool IsVisible(const AABB& box);
    };

    enum class ProjectionType
    {
        Perspective,
        Orthographic
    };

    class Camera
    {
        friend class CameraComponent;
        friend void Serialize(BinaryInputArchive& archive, Camera& camera);
        friend void Serialize(BinaryOutputArchive& archive, Camera& camera);
    public:
        Camera();
        ~Camera();

        [[nodiscard]]
        glm::mat4 GetProjection() const;
        [[nodiscard]]
        glm::mat4 GetViewMatrix() const;

        [[nodiscard]]
        glm::vec3 GetPosition() const;
        [[nodiscard]]
        glm::vec3 GetForward() const;
        [[nodiscard]]
        glm::vec3 GetUp() const;
        [[nodiscard]]
        glm::vec3 GetRight() const;

        void MoveForward(float speed);

        void MoveBackward(float speed);

        void MoveRight(float speed);

        void MoveLeft(float speed);

        void MoveUp(float speed);

        void MoveDown(float speed);

        void SetForward(const glm::vec3& forward);

        void AddPosition(const glm::vec3& position);

        void SetPosition(const glm::vec3& position);

        void AddPitch(float pitch);

        void AddYaw(float yaw);

        void SetCameraDistance(float distance);

        Frustum GetFrustum() const;

    private:
        glm::vec3 m_Position;
        glm::vec3 m_Forward;
        glm::vec3 m_Up;

        float m_Yaw, m_Pitch;
        float m_Fov;
        float m_CameraDistance;

        float OrthographicSize;
        float NearPlaneDistance;
        float FarPlaneDistance;

        ProjectionType m_ProjectionType;
    };
}

#endif //SUNSETCRAFT_CAMERA_H
