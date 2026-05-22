//
// Created by sunvy on 05/01/2026.
//

#include "RenderCommande.h"

#include "Core/Application.h"
#include "Core/ApplicationSetting.h"
#include "Drawable.h"
#include "Mesh.h"
#include "Shader.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
        std::shared_ptr<SunsetEngine::Material> material;
        glm::vec3 position;
        SunsetEngine::RenderState state;
    };

    // to change from just a vector to a 2 vector.
    std::vector<DrawCommand> m_DrawCommands;

    FrameData m_FrameData;

    GLenum ToGLBlendFactor(SunsetEngine::BlendFactor factor)
    {
        switch (factor)
        {
        case SunsetEngine::BlendFactor::Zero:                return GL_ZERO;
        case SunsetEngine::BlendFactor::One:                 return GL_ONE;
        case SunsetEngine::BlendFactor::SrcAlpha:            return GL_SRC_ALPHA;
        case SunsetEngine::BlendFactor::OneMinusSrcAlpha:    return GL_ONE_MINUS_SRC_ALPHA;
        case SunsetEngine::BlendFactor::DstAlpha:            return GL_DST_ALPHA;
        case SunsetEngine::BlendFactor::OneMinusDstAlpha:    return GL_ONE_MINUS_DST_ALPHA;
        case SunsetEngine::BlendFactor::SrcColor:            return GL_SRC_COLOR;
        case SunsetEngine::BlendFactor::OneMinusSrcColor:    return GL_ONE_MINUS_SRC_COLOR;
        default:                               return GL_ONE;
        }
    }

    GLenum ToGLCullMode(SunsetEngine::CullMode mode)
    {
        switch (mode)
        {
        case SunsetEngine::CullMode::Back:  return GL_BACK;
        case SunsetEngine::CullMode::Front: return GL_FRONT;
        default:              return GL_BACK;
        }
    }

    GLenum ToGLPrimitiveType(const SunsetEngine::PrimitiveType& type)
    {
        switch (type)
        {
        case SunsetEngine::PrimitiveType::Points:           return GL_POINT;
        case SunsetEngine::PrimitiveType::Lines:            return GL_LINES;
        case SunsetEngine::PrimitiveType::LineStrip:        return GL_LINE_STRIP;
        case SunsetEngine::PrimitiveType::Triangles:        return GL_TRIANGLES;
        case SunsetEngine::PrimitiveType::TriangleStrip:    return GL_TRIANGLE_STRIP;
        case SunsetEngine::PrimitiveType::TriangleFan:      return GL_TRIANGLE_FAN;
        }
        return GL_POINT;
    }

    void ApplyState(const SunsetEngine::RenderState& state)
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
            glBlendFunc(
                ToGLBlendFactor(state.src),
                ToGLBlendFactor(state.dest)
            );
        }
        else
        {
            glDisable(GL_BLEND);
        }

        // Face Culling
        if (state.cullMode == SunsetEngine::CullMode::None)
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

    void FlushDrawCommand()
    {
        HeapTest t(std::format("FlushDrawCommand {}", m_DrawCommands.size()));

        // Sort cmd
        std::shared_ptr<SunsetEngine::Shader> currentShader = nullptr;
        std::shared_ptr<SunsetEngine::Material> currentMaterial = nullptr;
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
                currentShader->SetVec3("camPosition", m_FrameData.position);
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

            cmd.material->m_Shader->SetVec3("location", cmd.position);

            if (cmd.state.DrawInstance)
                glDrawArraysInstanced(ToGLPrimitiveType(cmd.state.primitiveType), 0, 6, cmd.indexCount);
            else if (cmd.state.HasIndice)
                glDrawElements(ToGLPrimitiveType(cmd.state.primitiveType), cmd.indexCount, GL_UNSIGNED_INT, nullptr);
            else
                glDrawArrays(ToGLPrimitiveType(cmd.state.primitiveType), 0, cmd.indexCount);
        }

        m_DrawCommands.clear();
    }
}

namespace SunsetEngine
{
    void RenderCommande::BeginFrame()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void RenderCommande::EndFrame()
    {
        FlushDrawCommand();

        if (!PrintScreen::Get().empty())
        {
            ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
            ImGui::Begin("Stats", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
            for (const auto& it : PrintScreen::Get())
            {
                ImGui::Text("%s", it.c_str());
            }
            ImGui::End();
            PrintScreen::Clear();
        }

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(static_cast<GLFWwindow*>(Application::GetWindow()));
    }

    void RenderCommande::Submit(const SunsetEngine::Drawable& drawable)
    {
        DrawCommand cmd;
        cmd.vao = drawable.m_Mesh->GetVAO();
        cmd.indexCount = drawable.m_Mesh->GetVertexCount();
        cmd.material = drawable.m_Material;
        cmd.position = drawable.m_Position;
        cmd.state = drawable.m_RenderState;
        m_DrawCommands.emplace_back(cmd);
    }

    void RenderCommande::UseCamera(const SunsetEngine::Camera& camera)
    {
        m_FrameData.position = camera.GetPosition();
        m_FrameData.view = camera.GetViewMatrix();
        m_FrameData.projection = camera.GetProjection();
    }
}
