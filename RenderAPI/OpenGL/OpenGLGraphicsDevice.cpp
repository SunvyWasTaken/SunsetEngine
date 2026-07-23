//
// Created by sunvy on 16/07/2026.
//

#include "OpenGLGraphicsDevice.h"

#include "OpenGLDraw.h"
#include "OpenGLRendererState.h"

#include "Core/Application.h"
#include "Core/WindowSetting.h"
#include "Render/Camera.h"
#include "Render/FrameBuffer.h"
#include "Render/Meshes/Mesh.h"
#include "Render/Resources/Drawable.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Sunset
{
    OpenGLGraphicsDevice::~OpenGLGraphicsDevice()
    {
        m_Context.reset();
    }

    void OpenGLGraphicsDevice::Init()
    {
        auto* window = static_cast<GLFWwindow*>(Application::GetWindow());
        if (window == nullptr)
            throw std::runtime_error("OpenGL requires a window with a graphics context");

        m_Context = std::make_unique<OpenGLContext>(window);
        m_Context->Init();
    }

    void OpenGLGraphicsDevice::BeginFrame()
    {
        // FrameBuffer::Unbind();
        // SetViewport(Application::GetSetting().WindowSize);
        ResetFrameState();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLGraphicsDevice::EndFrame()
    {
        Flush();
        ResetFrameState();
        m_Context->SwapBuffers();
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
        m_DrawQueue.Submit(drawable, model);
    }

    void OpenGLGraphicsDevice::DrawMesh(const Mesh& mesh, const RenderState& state)
    {
        ApplyState(state);
        BindVertexArray(mesh.GetRenderHandle());
        DrawBoundMesh(mesh.GetVertexCount(), state);
    }

    void OpenGLGraphicsDevice::UseCamera(const Camera& camera)
    {
        m_DrawQueue.UseCamera(camera);
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

    void OpenGLGraphicsDevice::SetCullMode(const CullMode mode)
    {
        OpenGLRendererState::SetCullMode(mode);
    }

    void OpenGLGraphicsDevice::ResetFrameState() const
    {
        OpenGLRendererState::Reset();
    }

    void OpenGLGraphicsDevice::ApplyState(const RenderState& state) const
    {
        OpenGLRendererState::Apply(state);
    }

    void OpenGLGraphicsDevice::DrawBoundMesh(const std::uint32_t vertexCount, const RenderState& state) const
    {
        OpenGLDraw::Draw(vertexCount, state);
    }

    void OpenGLGraphicsDevice::FlushDrawCommands()
    {
        m_DrawQueue.Flush();
    }
} // Sunset
