//
// Created by Codex on 23/07/2026.
//

#include "OpenGLDrawQueue.h"

#include "OpenGLDraw.h"
#include "OpenGLRendererState.h"
#include "OpenGLVertexArray.h"
#include "Render/Camera.h"
#include "Render/Core/Shader.h"
#include "Render/Meshes/Mesh.h"
#include "Render/Resources/Drawable.h"
#include "Render/Resources/Material.h"

#include <algorithm>
#include <tuple>

namespace Sunset
{
    void OpenGLDrawQueue::Submit(const Drawable& drawable, const glm::mat4& model)
    {
        if (!drawable)
            return;

        DrawCommand cmd;
        cmd.vertexArray = drawable.m_Mesh->GetRenderHandle();
        cmd.indexCount = drawable.m_Mesh->GetVertexCount();
        cmd.material = drawable.m_Material;
        cmd.model = model;
        cmd.state = drawable.m_RenderState;
        m_DrawCommands.emplace_back(cmd);
    }

    void OpenGLDrawQueue::UseCamera(const Camera& camera)
    {
        m_FrameData.position = camera.GetPosition();
        m_FrameData.view = camera.GetViewMatrix();
        m_FrameData.projection = camera.GetProjection();
    }

    void OpenGLDrawQueue::Flush()
    {
        SS_PROFILE_FUNCTION();
        Sort();

        std::shared_ptr<Shader> currentShader = nullptr;
        std::shared_ptr<Material> currentMaterial = nullptr;
        std::uint32_t currentVertexArray = 0;

        for (const DrawCommand& cmd : m_DrawCommands)
        {
            OpenGLRendererState::Apply(cmd.state);

            if (currentShader != cmd.material->m_Shader)
            {
                currentShader = cmd.material->m_Shader;
                currentShader->Use();
                currentShader->SetMat4("view", m_FrameData.view);
                currentShader->SetMat4("projection", m_FrameData.projection);
                currentShader->SetVec3("u_CameraPos", m_FrameData.position);
            }

            if (currentMaterial != cmd.material)
            {
                currentMaterial = cmd.material;
                currentMaterial->Bind();
            }

            if (currentVertexArray != cmd.vertexArray)
            {
                currentVertexArray = cmd.vertexArray;
                OpenGLVertexArray::Bind(currentVertexArray);
            }

            cmd.material->UniformBind();
            cmd.material->m_Shader->SetMat4("model", cmd.model);
            OpenGLDraw::Draw(cmd.indexCount, cmd.state);
        }

        m_DrawCommands.clear();
    }

    void OpenGLDrawQueue::Sort()
    {
        std::stable_sort(m_DrawCommands.begin(), m_DrawCommands.end(), [this](const DrawCommand& lhs, const DrawCommand& rhs)
        {
            const bool lhsOverlay = lhs.state.layer == RenderLayer::Overlay;
            const bool rhsOverlay = rhs.state.layer == RenderLayer::Overlay;
            if (lhsOverlay != rhsOverlay)
                return !lhsOverlay;
            if (lhsOverlay)
                return false;

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

            return std::tie(lhs.material->m_Shader, lhs.material, lhs.vertexArray) < std::tie(rhs.material->m_Shader, rhs.material, rhs.vertexArray);
        });
    }
}
