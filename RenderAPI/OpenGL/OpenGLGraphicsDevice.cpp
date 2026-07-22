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
#include "Render/BufferObject/Buffers.h"

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

    GLenum ToGLBufferTarget(const Sunset::BufferType type)
    {
        switch (type)
        {
            case Sunset::BufferType::Vertex: return GL_ARRAY_BUFFER;
            case Sunset::BufferType::Index:  return GL_ELEMENT_ARRAY_BUFFER;
            case Sunset::BufferType::Uniform: return GL_UNIFORM_BUFFER;
        }
        return GL_ARRAY_BUFFER;
    }

    GLenum ToGLBufferUsage(const Sunset::BufferUsage usage)
    {
        switch (usage)
        {
            case Sunset::BufferUsage::Static:  return GL_STATIC_DRAW;
            case Sunset::BufferUsage::Dynamic: return GL_DYNAMIC_DRAW;
        }
        return GL_STATIC_DRAW;
    }

    GLenum ToGLShaderComponentType(const Sunset::ShaderComponentType type)
    {
        switch (type)
        {
            case Sunset::ShaderComponentType::Float: return GL_FLOAT;
            case Sunset::ShaderComponentType::Int:   return GL_INT;
            case Sunset::ShaderComponentType::UInt:  return GL_UNSIGNED_INT;
            case Sunset::ShaderComponentType::Bool:  return GL_BOOL;
        }
        return GL_FLOAT;
    }

    GLenum ToGLTextureFormat(const Sunset::TextureFormat format)
    {
        switch (format)
        {
            case Sunset::TextureFormat::Red:  return GL_RED;
            case Sunset::TextureFormat::RGB:  return GL_RGB;
            case Sunset::TextureFormat::RGBA: return GL_RGBA;
        }
        return GL_RGB;
    }
}

namespace Sunset
{
    OpenGLGraphicsDevice::~OpenGLGraphicsDevice()
    {
        SRmGUI::Opengl_Shutdown();
    }

    void OpenGLGraphicsDevice::Init()
    {
        auto* window = static_cast<GLFWwindow*>(Application::GetWindow());
        if (window == nullptr)
            throw std::runtime_error("OpenGL requires a window with a graphics context");

        glfwMakeContextCurrent(window);
        if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == GL_FALSE)
            throw std::runtime_error("Failed to initialize GLAD");

        SRmGUI::Opengl_Init();
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
        cmd.vertexArray = drawable.m_Mesh->GetRenderHandle();
        cmd.indexCount = drawable.m_Mesh->GetVertexCount();
        cmd.material = drawable.m_Material;
        cmd.model = model;
        cmd.state = drawable.m_RenderState;
        m_DrawCommands.emplace_back(cmd);
    }

    void OpenGLGraphicsDevice::DrawMesh(const Mesh& mesh, const RenderState& state)
    {
        ApplyState(state);
        BindVertexArray(mesh.GetRenderHandle());
        DrawBoundMesh(mesh.GetVertexCount(), state);
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

    std::uint32_t OpenGLGraphicsDevice::CreateBuffer(const BufferType type, const void* data, const size_t size, const BufferUsage usage)
    {
        std::uint32_t buffer = 0;
        const GLenum target = ToGLBufferTarget(type);
        glGenBuffers(1, &buffer);
        glBindBuffer(target, buffer);
        glBufferData(target, size, data, ToGLBufferUsage(usage));
        glBindBuffer(target, 0);
        return buffer;
    }

    void OpenGLGraphicsDevice::DestroyBuffer(const std::uint32_t buffer)
    {
        glDeleteBuffers(1, &buffer);
    }

    void OpenGLGraphicsDevice::BindBuffer(const BufferType type, const std::uint32_t buffer)
    {
        glBindBuffer(ToGLBufferTarget(type), buffer);
    }

    void OpenGLGraphicsDevice::UpdateBuffer(const BufferType type, const std::uint32_t buffer, const size_t offset, const size_t size, const void* data)
    {
        const GLenum target = ToGLBufferTarget(type);
        glBindBuffer(target, buffer);
        glBufferSubData(target, offset, size, data);
        glBindBuffer(target, 0);
    }

    std::uint32_t OpenGLGraphicsDevice::CreateVertexArray()
    {
        std::uint32_t vertexArray = 0;
        glGenVertexArrays(1, &vertexArray);
        return vertexArray;
    }

    void OpenGLGraphicsDevice::DestroyVertexArray(const std::uint32_t vertexArray)
    {
        glDeleteVertexArrays(1, &vertexArray);
    }

    void OpenGLGraphicsDevice::BindVertexArray(const std::uint32_t vertexArray)
    {
        glBindVertexArray(vertexArray);
    }

    void OpenGLGraphicsDevice::ConfigureVertexArray(const std::uint32_t vertexArray, const std::uint32_t vertexBuffer, const BufferLayout& layout)
    {
        BindVertexArray(vertexArray);
        BindBuffer(BufferType::Vertex, vertexBuffer);

        std::uint32_t index = 0;
        for (const BufferElement& element : layout)
        {
            glEnableVertexAttribArray(index);
            const auto offset = reinterpret_cast<const void*>(element.offset);
            const GLenum componentType = ToGLShaderComponentType(element.ComponentType());
            if (element.IsInt())
                glVertexAttribIPointer(index, element.Count(), componentType, layout.GetStride(), offset);
            else
                glVertexAttribPointer(index, element.Count(), componentType, element.normalized, layout.GetStride(), offset);

            if (element.divisor > 0)
                glVertexAttribDivisor(index, element.divisor);

            ++index;
        }

        BindVertexArray(0);
        BindBuffer(BufferType::Vertex, 0);
    }

    std::uint32_t OpenGLGraphicsDevice::CreateTexture2D(const TextureSpecification& specification, const void* data)
    {
        std::uint32_t texture = 0;
        const GLenum format = ToGLTextureFormat(specification.format);
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, format, specification.width, specification.height, 0, format, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);
        return texture;
    }

    void OpenGLGraphicsDevice::DestroyTexture(const std::uint32_t texture)
    {
        glDeleteTextures(1, &texture);
    }

    void OpenGLGraphicsDevice::BindTexture(const std::uint32_t texture, const std::uint32_t slot)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    void OpenGLGraphicsDevice::UpdateTexture2D(const std::uint32_t texture, const glm::ivec2& coord, const TextureSpecification& specification, const void* data)
    {
        const GLenum format = ToGLTextureFormat(specification.format);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, coord.x, coord.y, specification.width, specification.height, format, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);
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

    void OpenGLGraphicsDevice::DrawBoundMesh(const std::uint32_t vertexCount, const RenderState& state) const
    {
        if (state.DrawInstance)
            glDrawArraysInstanced(ToGLPrimitiveType(state.primitiveType), 0, state.nbrInstance, vertexCount);
        else if (state.HasIndice)
            glDrawElements(ToGLPrimitiveType(state.primitiveType), vertexCount, GL_UNSIGNED_INT, nullptr);
        else
            glDrawArrays(ToGLPrimitiveType(state.primitiveType), 0, vertexCount);
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

            return std::tie(lhs.material->m_Shader, lhs.material, lhs.vertexArray) < std::tie(rhs.material->m_Shader, rhs.material, rhs.vertexArray);
        });
    }

    void OpenGLGraphicsDevice::FlushDrawCommands()
    {
        SS_PROFILE_FUNCTION();
        SortDrawCommands();

        std::shared_ptr<Shader> currentShader = nullptr;
        std::shared_ptr<Material> currentMaterial = nullptr;
        GLuint currentVertexArray = 0;

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

            if (currentVertexArray != cmd.vertexArray)
            {
                currentVertexArray = cmd.vertexArray;
                BindVertexArray(currentVertexArray);
            }

            cmd.material->UniformBind();
            cmd.material->m_Shader->SetMat4("model", cmd.model);

            DrawBoundMesh(cmd.indexCount, cmd.state);
        }

        m_DrawCommands.clear();
    }
} // Sunset
