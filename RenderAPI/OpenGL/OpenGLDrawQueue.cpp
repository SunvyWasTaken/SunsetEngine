//
// Created by sunvy on 25/08/2026.
//

#include "OpenGLDrawQueue.h"

#include <glad/glad.h>

#include "Render/Resources/Buffer.h"
#include "Render/Resources/Camera.h"
#include "Render/Resources/Drawable.h"
#include "Render/Resources/Material.h"
#include "Render/Resources/Mesh.h"

namespace Sunset
{
    OpenGLDrawQueue::OpenGLDrawQueue()
        : m_DrawCommands()
        , m_FrameData()
    {
    }

    void OpenGLDrawQueue::Submit(const Drawable &drawable, const glm::mat4 &model)
    {
        m_DrawCommands.emplace_back(DrawCommand{drawable.m_Mesh, drawable.m_Material, model});
    }

    void OpenGLDrawQueue::UseCamera(const Camera &camera)
    {
        m_FrameData.position = camera.GetPosition();
        m_FrameData.view = camera.GetViewMatrix();
        m_FrameData.projection = camera.GetProjection();
    }

    void OpenGLDrawQueue::Flush()
    {
        SS_PROFILE_FUNCTION();
        Sort();
        for (const auto& cmd : m_DrawCommands)
        {
            if (!cmd.material || !cmd.mesh)
                continue;

            cmd.material->Bind();

            cmd.mesh->Bind();

            if (cmd.mesh->m_IndexBuffer)
                glDrawElements(GL_TRIANGLES, cmd.mesh->m_IndexBuffer->Count(), GL_UNSIGNED_INT, nullptr);
            else
                glDrawArrays(GL_TRIANGLES, 0, cmd.mesh->m_VertexBuffer->Count());
        }
        m_DrawCommands.clear();
    }

    void OpenGLDrawQueue::Sort()
    {
        SS_PROFILE_FUNCTION();
        /// Todo : Redo the sort with the new DrawCmd.
    }
} // Sunset