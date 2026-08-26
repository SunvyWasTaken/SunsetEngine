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
#include "Render/Resources/Pipeline.h"
#include "Render/Resources/Shader.h"

namespace
{
    void CheckOpenGLError(const char* location)
    {
        const GLenum error = glGetError();
        while (error != GL_NO_ERROR)
        {
            LOG("OpenGL", error, "OpenGL error at {}: 0x{:X}", location, static_cast<unsigned int>(error))
        }
    }
}

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
        PRINTSCREEN("Flush {} draw commands", m_DrawCommands.size());
        for (const auto& cmd : m_DrawCommands)
        {
            if (!cmd.material || !cmd.mesh)
            {
                PRINTSCREEN("Draw cmd ignored");
                continue;
            }

            cmd.material->m_Pipeline->Bind();

            cmd.material->Bind();

            cmd.material->m_Shader->SetVec3("u_CameraPos", m_FrameData.position);
            cmd.material->m_Shader->SetMat4("view", m_FrameData.view);
            cmd.material->m_Shader->SetMat4("projection", m_FrameData.projection);
            cmd.material->m_Shader->SetMat4("model", cmd.model);

            cmd.material->UniformBind();

            cmd.mesh->Bind();

            CheckOpenGLError("Before draw");
            if (cmd.mesh->m_IndexBuffer)
                glDrawElements(GL_TRIANGLES, cmd.mesh->m_IndexBuffer->Count(), GL_UNSIGNED_INT, nullptr);
            else
                glDrawArrays(GL_TRIANGLES, 0, cmd.mesh->m_VertexBuffer->Count());
            CheckOpenGLError("After draw");
        }
        m_DrawCommands.clear();
    }

    void OpenGLDrawQueue::Sort()
    {
        SS_PROFILE_FUNCTION();
        /// Todo : Redo the sort with the new DrawCmd.
    }
} // Sunset