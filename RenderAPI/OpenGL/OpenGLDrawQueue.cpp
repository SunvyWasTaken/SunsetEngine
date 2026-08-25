//
// Created by sunvy on 25/08/2026.
//

#include "OpenGLDrawQueue.h"

#include "Render/Object/Camera.h"

namespace Sunset
{
    void OpenGLDrawQueue::Submit()
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
    }
} // Sunset