//
// Created by sunvy on 15/12/2025.
//

#ifndef SUNSETCRAFT_CAMERA_H
#define SUNSETCRAFT_CAMERA_H
#include "Drawable.h"

namespace Sunset
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
        // A camera renders every submitted drawable matching this mask into frameBufferId.
        // frameBufferId == 0 renders to the default back buffer. The camera does not
        // need to be attached to an entity to render an isolated pass.
        explicit Camera(RenderPassMask renderMask = RenderPass::Main, uint32_t frameBufferId = 0, bool active = true);
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

        void MoveDown(float speed);

        void SetForward(const glm::vec3& forward);

        void AddPosition(const glm::vec3& position);

        void SetPosition(const glm::vec3& position);

        void AddPitch(float pitch);

        void AddYaw(float yaw);

        Frustum GetFrustum() const;

        void Render(bool clear = true) const;

        void SetActive(bool active);
        bool IsActive() const;

        void SetRenderMask(RenderPassMask renderMask);
        RenderPassMask GetRenderMask() const;

        void SetFrameBufferId(uint32_t frameBufferId);
        uint32_t GetFrameBufferId() const;

    private:
        glm::vec3 m_Position;
        glm::vec3 m_Forward;
        glm::vec3 m_Up;

        float m_Yaw, m_Pitch;

        float fov;

        bool m_Active;
        RenderPassMask m_RenderMask;
        uint32_t m_FrameBufferId;
    };
}

#endif //SUNSETCRAFT_CAMERA_H
