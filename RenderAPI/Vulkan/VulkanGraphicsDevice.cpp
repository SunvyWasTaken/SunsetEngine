//
// Created by Codex on 23/07/2026.
//

#include "VulkanGraphicsDevice.h"

#include "VulkanCommandContext.h"
#include "VulkanDevice.h"
#include "VulkanInstance.h"
#include "VulkanResourceRegistry.h"
#include "VulkanSurface.h"
#include "VulkanSwapchain.h"

#include "Core/Application.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <string>

namespace Sunset
{
    VulkanGraphicsDevice::VulkanGraphicsDevice() = default;

    VulkanGraphicsDevice::~VulkanGraphicsDevice()
    {
        if (m_Device)
            vkDeviceWaitIdle(m_Device->GetDevice());
    }

    void VulkanGraphicsDevice::Init()
    {
        void* nativeWindow = Application::GetWindow();
        if (nativeWindow == nullptr)
            throw std::runtime_error("Vulkan requires a native window");

        m_Instance = std::make_unique<VulkanInstance>(Application::GetSetting().WindowTitle);
        m_Surface = std::make_unique<VulkanSurface>(m_Instance->Get(), nativeWindow);
        m_Device = std::make_unique<VulkanDevice>(m_Instance->Get(), m_Surface->Get());
        m_Swapchain = std::make_unique<VulkanSwapchain>(*m_Device, m_Surface->Get(), Application::GetSetting().WindowSize);
        m_CommandContext = std::make_unique<VulkanCommandContext>(*m_Device, *m_Swapchain);
        m_Resources = std::make_unique<VulkanResourceRegistry>();
    }

    void VulkanGraphicsDevice::BeginFrame()
    {
        RequireInitialized("BeginFrame");
        m_CommandContext->BeginFrame();
    }

    void VulkanGraphicsDevice::EndFrame()
    {
        RequireInitialized("EndFrame");
        m_CommandContext->EndFrame();
    }

    void VulkanGraphicsDevice::BeginTarget(FrameBuffer&, const glm::vec4& clearColor)
    {
        if (m_CommandContext)
            m_CommandContext->SetClearColor(clearColor);
    }

    void VulkanGraphicsDevice::EndTarget() {}
    void VulkanGraphicsDevice::Flush() {}
    void VulkanGraphicsDevice::Submit(const Drawable&, const glm::mat4&) {}
    void VulkanGraphicsDevice::DrawMesh(const Mesh&, const RenderState&) {}
    void VulkanGraphicsDevice::UseCamera(const Camera&) {}
    void VulkanGraphicsDevice::ShowCursor(const bool show)
    {
        auto* window = static_cast<GLFWwindow*>(Application::GetWindow());
        if (window != nullptr)
            glfwSetInputMode(window, GLFW_CURSOR, !show ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }

    void VulkanGraphicsDevice::SetViewport(const glm::ivec2&) {}

    void VulkanGraphicsDevice::Clear(const glm::vec4& color)
    {
        RequireInitialized("Clear");
        m_CommandContext->SetClearColor(color);
    }

    void VulkanGraphicsDevice::SetCullMode(CullMode) {}

    FrameBufferCreateResult VulkanGraphicsDevice::CreateFrameBuffer(const FrameBufferSpecification& specification)
    {
        return RequireResources("CreateFrameBuffer").CreateFrameBuffer(specification);
    }

    void VulkanGraphicsDevice::DestroyFrameBuffer(const FrameBufferHandle framebuffer, const std::vector<TextureHandle>&, TextureHandle)
    {
        if (m_Resources)
            m_Resources->DestroyFrameBuffer(framebuffer);
    }

    void VulkanGraphicsDevice::BindFrameBuffer(FrameBufferHandle, const glm::ivec2&) {}
    void VulkanGraphicsDevice::UnbindFrameBuffer() {}

    void VulkanGraphicsDevice::ClearFrameBuffer(const FrameBuffer&, ClearFlags, const glm::vec4& color, float, int)
    {
        if (m_CommandContext)
            m_CommandContext->SetClearColor(color);
    }

    void VulkanGraphicsDevice::ClearFrameBufferColor(const FrameBuffer&, std::uint32_t, const glm::vec4& color)
    {
        if (m_CommandContext)
            m_CommandContext->SetClearColor(color);
    }

    void VulkanGraphicsDevice::ClearFrameBufferDepth(const FrameBuffer&, float) {}
    void VulkanGraphicsDevice::ClearFrameBufferStencil(const FrameBuffer&, int) {}
    void VulkanGraphicsDevice::BlitFrameBuffer(const FrameBuffer&, FrameBuffer&, ClearFlags) {}

    bool VulkanGraphicsDevice::IsFrameBufferValid(const FrameBufferHandle framebuffer)
    {
        return m_Resources && m_Resources->IsFrameBufferValid(framebuffer);
    }

    BufferHandle VulkanGraphicsDevice::CreateBuffer(const BufferType type, const void* data, const size_t size, const BufferUsage usage)
    {
        return RequireResources("CreateBuffer").CreateBuffer(type, data, size, usage);
    }

    void VulkanGraphicsDevice::DestroyBuffer(const BufferHandle buffer)
    {
        if (m_Resources)
            m_Resources->DestroyBuffer(buffer);
    }

    void VulkanGraphicsDevice::BindBuffer(BufferType, BufferHandle) {}

    void VulkanGraphicsDevice::UpdateBuffer(BufferType, const BufferHandle buffer, const size_t offset, const size_t size, const void* data)
    {
        if (m_Resources)
            m_Resources->UpdateBuffer(buffer, offset, size, data);
    }

    VertexArrayHandle VulkanGraphicsDevice::CreateVertexArray()
    {
        return RequireResources("CreateVertexArray").CreateVertexArray();
    }

    void VulkanGraphicsDevice::DestroyVertexArray(const VertexArrayHandle vertexArray)
    {
        if (m_Resources)
            m_Resources->DestroyVertexArray(vertexArray);
    }

    void VulkanGraphicsDevice::BindVertexArray(VertexArrayHandle) {}

    void VulkanGraphicsDevice::ConfigureVertexArray(const VertexArrayHandle vertexArray, const BufferHandle vertexBuffer, const BufferLayout& layout)
    {
        if (m_Resources)
            m_Resources->ConfigureVertexArray(vertexArray, vertexBuffer, layout);
    }

    TextureHandle VulkanGraphicsDevice::CreateTexture2D(const TextureSpecification& specification, const void* data)
    {
        return RequireResources("CreateTexture2D").CreateTexture2D(specification, data);
    }

    void VulkanGraphicsDevice::DestroyTexture(const TextureHandle texture)
    {
        if (m_Resources)
            m_Resources->DestroyTexture(texture);
    }

    void VulkanGraphicsDevice::BindTexture(TextureHandle, std::uint32_t) {}

    void VulkanGraphicsDevice::UpdateTexture2D(const TextureHandle texture, const glm::ivec2&, const TextureSpecification& specification, const void* data)
    {
        if (m_Resources)
            m_Resources->UpdateTexture2D(texture, specification, data);
    }

    ShaderHandle VulkanGraphicsDevice::CreateShader(const std::string_view vertexSource, const std::string_view fragmentSource)
    {
        return RequireResources("CreateShader").CreateShader(vertexSource, fragmentSource);
    }

    void VulkanGraphicsDevice::DestroyShader(const ShaderHandle shader)
    {
        if (m_Resources)
            m_Resources->DestroyShader(shader);
    }

    void VulkanGraphicsDevice::BindShader(ShaderHandle) {}
    void VulkanGraphicsDevice::SetShaderFloat(ShaderHandle, std::string_view, float) {}
    void VulkanGraphicsDevice::SetShaderInt(ShaderHandle, std::string_view, int) {}
    void VulkanGraphicsDevice::SetShaderVec2(ShaderHandle, std::string_view, const glm::vec2&) {}
    void VulkanGraphicsDevice::SetShaderVec3(ShaderHandle, std::string_view, const glm::vec3&) {}
    void VulkanGraphicsDevice::SetShaderVec4(ShaderHandle, std::string_view, const glm::vec4&) {}
    void VulkanGraphicsDevice::SetShaderMat4(ShaderHandle, std::string_view, const glm::mat4&) {}

    void VulkanGraphicsDevice::RequireInitialized(const std::string_view method) const
    {
        if (!m_CommandContext)
            throw std::runtime_error(std::string("VulkanGraphicsDevice::") + std::string(method) + " called before Init");
    }

    VulkanResourceRegistry& VulkanGraphicsDevice::RequireResources(const std::string_view method) const
    {
        if (!m_Resources)
            throw std::runtime_error(std::string("VulkanGraphicsDevice::") + std::string(method) + " called before Init");

        return *m_Resources;
    }
}
