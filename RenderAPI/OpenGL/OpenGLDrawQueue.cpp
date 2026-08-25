//
// Created by sunvy on 25/08/2026.
//

#include "OpenGLDrawQueue.h"

#include "Render/Resources/Camera.h"

namespace Sunset
{
    void OpenGLDrawQueue::Submit(const Drawable &drawable, const glm::mat4 &model)
    {
    }

    void OpenGLDrawQueue::UseCamera(const Camera &camera)
    {
        m_FrameData.position = camera.GetPosition();
        m_FrameData.view = camera.GetViewMatrix();
        m_FrameData.projection = camera.GetProjection();
    }

    void OpenGLDrawQueue::Flush()
    {
        Sort();
        SS_PROFILE_FUNCTION();
    }

    void OpenGLDrawQueue::Sort()
    {
        SS_PROFILE_FUNCTION();
        std::stable_sort(m_DrawCommands.begin(), m_DrawCommands.begin(), [this](const DrawCommand &lhs, const DrawCommand &rhs)
        {
            const bool lhsTransparent = lhs.state.blending;
            const bool rhsTransparent = rhs.state.blending;
            if (lhsTransparent != rhsTransparent)
                return !lhsTransparent;
            if (lhsTransparent)
            {
                const glm::vec3 lhsPosition = glm::vec3(lhs.model[3]);
                const glm::vec3 rhsPosition = glm::vec3(rhs.model[3]);
                const glm::vec3 lhsDistance = lhsPosition - m_FrameData.position;
                const glm::vec3 rhsDistance = rhsPosition - m_FrameData.position;
                return glm::dot(lhsDistance, lhsDistance) > glm::dot(rhsDistance, rhsDistance);
            }

            return std::tie(lhs.material->m_Shader, lhs.material, lhs.vertexArray.id) < std::tie(rhs.material->m_Shader, rhs.material, rhs.vertexArray.id);
                    });
    }
} // Sunset