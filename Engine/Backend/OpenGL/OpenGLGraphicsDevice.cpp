//
// Created by sunvy on 16/07/2026.
//

#include "OpenGLGraphicsDevice.h"

#include "Core/Application.h"
#include "Core/WindowSetting.h"
#include "Render/Camera.h"
#include "Render/FrameBuffer.h"
#include "Render/Meshes/Mesh.h"
#include "Render/Resources/Drawable.h"
#include "Render/Resources/Material.h"
#include "Render/Core/Shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <algorithm>
#include <tuple>

#include "SRmGUI.h"
#include "SRmGUI_Opengl.h"

namespace
{
    GLenum ToGLBlendFactor(Sunset::BlendFactor factor)
    {
        switch (factor)
        {
            case Sunset::BlendFactor::Zero:                return GL_ZERO;
            case Sunset::BlendFactor::One:                 return GL_ONE;
            case Sunset::BlendFactor::SrcAlpha:            return GL_SRC_ALPHA;
            case Sunset::BlendFactor::OneMinusSrcAlpha:    return GL_ONE_MINUS_SRC_ALPHA;
            case Sunset::BlendFactor::DstAlpha:            return GL_DST_ALPHA;
            case Sunset::BlendFactor::OneMinusDstAlpha:    return GL_ONE_MINUS_DST_ALPHA;
            case Sunset::BlendFactor::SrcColor:            return GL_SRC_COLOR;
            case Sunset::BlendFactor::OneMinusSrcColor:    return GL_ONE_MINUS_SRC_COLOR;
            case Sunset::BlendFactor::DstColor:            return GL_DST_COLOR;
            case Sunset::BlendFactor::OneMinusDstColor:    return GL_ONE_MINUS_DST_COLOR;
            default:                                       return GL_ONE;
        }
    }

    GLenum ToGLCullMode(Sunset::CullMode mode)
    {
        switch (mode)
        {
            case Sunset::CullMode::Back:  return GL_BACK;
            case Sunset::CullMode::Front: return GL_FRONT;
            default:                      return GL_BACK;
        }
    }

    GLenum ToGLPrimitiveType(Sunset::PrimitiveType type)
    {
        switch (type)
        {
            case Sunset::PrimitiveType::Points:        return GL_POINTS;
            case Sunset::PrimitiveType::Lines:         return GL_LINES;
            case Sunset::PrimitiveType::LineStrip:     return GL_LINE_STRIP;
            case Sunset::PrimitiveType::Triangles:     return GL_TRIANGLES;
            case Sunset::PrimitiveType::TriangleStrip: return GL_TRIANGLE_STRIP;
            case Sunset::PrimitiveType::TriangleFan:   return GL_TRIANGLE_FAN;
        }
        return GL_POINTS;
    }
}

namespace Sunset
{
    void OpenGLGraphicsDevice::Init() {}

    void OpenGLGraphicsDevice::BeginFrame()
    {
        FrameBuffer::Unbind();
        SetViewport(Application::GetSetting().WindowSize);
        ResetFrameState();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLGraphicsDevice::EndFrame()
    {
        Flush();
        ResetFrameState();
        SRmGUI::Opengl_DrawData(SRmGUI::GetDrawData());
    }

    void OpenGLGraphicsDevice::BeginTarget(FrameBuffer& target, const glm::vec4& clearColor)
    {
        Flush();
        target.Clear(clearColor);
        ResetFrameState();
    }

    void OpenGLGraphicsDevice::EndTarget()
    {
        Flush();
        FrameBuffer::Unbind();
        SetViewport(Application::GetSetting().WindowSize);
        ResetFrameState();
    }

    void OpenGLGraphicsDevice::Flush()
    {
        FlushDrawCommands();
    }

    void OpenGLGraphicsDevice::Submit(const Drawable& drawable, const glm::mat4& model)
    {
        if (!drawable)
            return;

        DrawCommand cmd;
        cmd.vao = drawable.m_Mesh->GetVAO();
        cmd.indexCount = drawable.m_Mesh->GetVertexCount();
        cmd.material = drawable.m_Material;
        cmd.model = model;
        cmd.state = drawable.m_RenderState;
        m_DrawCommands.emplace_back(cmd);
    }

    void OpenGLGraphicsDevice::UseCamera(const Camera& camera)
    {
        m_FrameData.position = camera.GetPosition();
        m_FrameData.view = camera.GetViewMatrix();
        m_FrameData.projection = camera.GetProjection();
    }

    void OpenGLGraphicsDevice::ShowCursor(bool show)
    {
        glfwSetInputMode(static_cast<GLFWwindow*>(Application::GetWindow()), GLFW_CURSOR, !show ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }

    void OpenGLGraphicsDevice::SetViewport(const glm::ivec2& size)
    {
        glViewport(0, 0, size.x, size.y);
    }

    void OpenGLGraphicsDevice::Clear(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLGraphicsDevice::ResetFrameState() const
    {
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        glDisable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    void OpenGLGraphicsDevice::ApplyState(const RenderState& state) const
    {
        state.depthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
        glDepthMask(state.depthWrite ? GL_TRUE : GL_FALSE);

        if (state.blending)
        {
            glEnable(GL_BLEND);
            glBlendEquation(GL_FUNC_ADD);
            glBlendFuncSeparate(ToGLBlendFactor(state.src), ToGLBlendFactor(state.dest), GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        }
        else
        {
            glDisable(GL_BLEND);
        }

        if (state.cullMode == CullMode::None)
        {
            glDisable(GL_CULL_FACE);
        }
        else
        {
            glEnable(GL_CULL_FACE);
            glCullFace(ToGLCullMode(state.cullMode));
        }

        glPolygonMode(GL_FRONT_AND_BACK, state.wireframe ? GL_LINE : GL_FILL);
    }

    void OpenGLGraphicsDevice::SortDrawCommands()
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

            return std::tie(lhs.material->m_Shader, lhs.material, lhs.vao) < std::tie(rhs.material->m_Shader, rhs.material, rhs.vao);
        });
    }

    void OpenGLGraphicsDevice::FlushDrawCommands()
    {
        SS_PROFILE_FUNCTION();
        SortDrawCommands();

        std::shared_ptr<Shader> currentShader = nullptr;
        std::shared_ptr<Material> currentMaterial = nullptr;
        GLuint currentVAO = 0;

        for (const DrawCommand& cmd : m_DrawCommands)
        {
            ApplyState(cmd.state);

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

            if (currentVAO != cmd.vao)
            {
                currentVAO = cmd.vao;
                glBindVertexArray(currentVAO);
            }

            cmd.material->UniformBind();
            cmd.material->m_Shader->SetMat4("model", cmd.model);

            if (cmd.state.DrawInstance)
                glDrawArraysInstanced(ToGLPrimitiveType(cmd.state.primitiveType), 0, cmd.state.nbrInstance, cmd.indexCount);
            else if (cmd.state.HasIndice)
                glDrawElements(ToGLPrimitiveType(cmd.state.primitiveType), cmd.indexCount, GL_UNSIGNED_INT, nullptr);
            else
                glDrawArrays(ToGLPrimitiveType(cmd.state.primitiveType), 0, cmd.indexCount);
        }

        m_DrawCommands.clear();
    }
} // Sunset