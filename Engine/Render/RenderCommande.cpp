//
// Created by sunvy on 05/01/2026.
//

#include "RenderCommande.h"

#include "Core/Application.h"
#include "Core/ApplicationSetting.h"
#include "Drawable.h"
#include "Meshes/Mesh.h"
#include "Shader.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>

#include "Camera.h"
#include "Material.h"

namespace
{
    struct FrameData
    {
        glm::vec3 position;
        glm::mat4 view;
        glm::mat4 projection;
    };

    struct DrawCommand
    {
        uint32_t vao;
        uint32_t indexCount;
        std::shared_ptr<Sunset::Material> material;
        glm::mat4 model;
        Sunset::RenderState state;
    };

    // to change from just a vector to a 2 vector.
    std::vector<DrawCommand> m_DrawCommands;

    FrameData m_FrameData;

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
        default:              return GL_BACK;
        }
    }

    GLenum ToGLPrimitiveType(const Sunset::PrimitiveType& type)
    {
        switch (type)
        {
        case Sunset::PrimitiveType::Points:           return GL_POINT;
        case Sunset::PrimitiveType::Lines:            return GL_LINES;
        case Sunset::PrimitiveType::LineStrip:        return GL_LINE_STRIP;
        case Sunset::PrimitiveType::Triangles:        return GL_TRIANGLES;
        case Sunset::PrimitiveType::TriangleStrip:    return GL_TRIANGLE_STRIP;
        case Sunset::PrimitiveType::TriangleFan:      return GL_TRIANGLE_FAN;
        }
        return GL_POINT;
    }

    void ResetFrameState()
    {
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        glDisable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    void ApplyState(const Sunset::RenderState& state)
    {
        // Depth Test
        if (state.depthTest)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);

        // Depth Write
        glDepthMask(state.depthWrite ? GL_TRUE : GL_FALSE);

        // Blending
        if (state.blending)
        {
            glEnable(GL_BLEND);
            glBlendEquation(GL_FUNC_ADD);
            glBlendFuncSeparate(
                ToGLBlendFactor(state.src),
                ToGLBlendFactor(state.dest),
                GL_ONE,
                GL_ONE_MINUS_SRC_ALPHA
            );
        }
        else
        {
            glDisable(GL_BLEND);
        }

        // Face Culling
        if (state.cullMode == Sunset::CullMode::None)
        {
            glDisable(GL_CULL_FACE);
        }
        else
        {
            glEnable(GL_CULL_FACE);
            glCullFace(ToGLCullMode(state.cullMode));
        }

        // Wireframe
        glPolygonMode(GL_FRONT_AND_BACK,
            state.wireframe ? GL_LINE : GL_FILL);
    }

    bool IsTransparent(const DrawCommand& cmd)
    {
        return cmd.state.blending;
    }

    float DistanceToCameraSquared(const DrawCommand& cmd)
    {
        const glm::vec3 cameraToObject = glm::vec3(cmd.model[3]) - m_FrameData.position;
        return glm::dot(cameraToObject, cameraToObject);
    }

    void SortDrawCommands()
    {
        std::stable_sort(m_DrawCommands.begin(), m_DrawCommands.end(), [](const DrawCommand& lhs, const DrawCommand& rhs)
        {
            const bool lhsTransparent = IsTransparent(lhs);
            const bool rhsTransparent = IsTransparent(rhs);

            if (lhsTransparent != rhsTransparent)
                return !lhsTransparent;

            if (lhsTransparent)
                return DistanceToCameraSquared(lhs) > DistanceToCameraSquared(rhs);

            return std::tie(lhs.material->m_Shader, lhs.material, lhs.vao)
                < std::tie(rhs.material->m_Shader, rhs.material, rhs.vao);
        });
    }

    void FlushDrawCommand()
    {
        SS_PROFILE_FUNCTION();
        // Sort cmd
        SortDrawCommands();

        std::shared_ptr<Sunset::Shader> currentShader = nullptr;
        std::shared_ptr<Sunset::Material> currentMaterial = nullptr;
        GLuint currentVAO = 0;

        // glEnable(GL_DEPTH_TEST);
        // glDepthMask(GL_TRUE);
        // glDepthFunc(GL_LESS);
        // glDepthRange(0.0, 1.0);

        for (const  DrawCommand& cmd : m_DrawCommands)
        {
            ApplyState(cmd.state);

            if (currentShader != cmd.material->m_Shader)
            {
                currentShader = cmd.material->m_Shader;
                currentShader->Use();
                currentShader->SetMat4("view", m_FrameData.view);
                currentShader->SetMat4("projection", m_FrameData.projection);
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
}

namespace Sunset
{
    void RenderCommande::BeginFrame()
    {
        ResetFrameState();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
    }

    void RenderCommande::EndFrame()
    {
        FlushDrawCommand();
        ResetFrameState();

        if (!PrintScreen::Get().empty())
        {
            ImGui::Begin("Stats", nullptr);
            for (const auto& it : PrintScreen::Get())
            {
                ImGui::Text("%s", it.c_str());
            }
            ImGui::End();
            PrintScreen::Clear();
        }
        if (!ProfileData::Get().empty())
        {
            ImGui::Begin("Profiling", nullptr);
            for (const auto& it : ProfileData::Get())
            {
                ImGui::Text("%s", it.c_str());
            }
            ImGui::End();
            ProfileData::Free();
        }
        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(static_cast<GLFWwindow*>(Application::GetWindow()));
    }

    void RenderCommande::Submit(const Drawable& drawable)
    {
        DrawCommand cmd;
        if (!drawable)
            return;

        cmd.vao = drawable.m_Mesh->GetVAO();
        cmd.indexCount = drawable.m_Mesh->GetVertexCount();
        cmd.material = drawable.m_Material;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, drawable.m_Position);
        model = glm::scale(model, drawable.m_Scale);
        cmd.model = model;
        cmd.state = drawable.m_RenderState;
        m_DrawCommands.emplace_back(cmd);
    }

    void RenderCommande::Submit(const Drawable &mesh, const glm::mat4 &transform)
    {
        DrawCommand cmd;
        if (!mesh)
            return;

        cmd.vao = mesh.m_Mesh->GetVAO();
        cmd.indexCount = mesh.m_Mesh->GetVertexCount();
        cmd.material = mesh.m_Material;

        cmd.model = transform;
        cmd.state = mesh.m_RenderState;
        m_DrawCommands.emplace_back(cmd);
    }

    void RenderCommande::UseCamera(const Camera& camera)
    {
        m_FrameData.position = camera.GetPosition();
        m_FrameData.view = camera.GetViewMatrix();
        m_FrameData.projection = camera.GetProjection();
    }
}
