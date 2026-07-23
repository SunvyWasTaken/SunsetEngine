//
// Created by Codex on 23/07/2026.
//

#pragma once

#include "Render/Core/RenderAPI.h"

#include <memory>

namespace Sunset
{
    class VulkanCommandContext;
    class VulkanDevice;
    class VulkanInstance;
    class VulkanResourceRegistry;
    class VulkanSurface;
    class VulkanSwapchain;

    class VulkanGraphicsDevice final : public RenderAPI
    {
    public:
        VulkanGraphicsDevice();
        ~VulkanGraphicsDevice() override;

        void Init() override;
        void BeginFrame() override;
        void EndFrame() override;

        void BeginTarget(FrameBuffer& target, const glm::vec4& clearColor) override;
        void EndTarget() override;
        void Flush() override;

        void Submit(const Drawable& drawable, const glm::mat4& model) override;
        void DrawMesh(const Mesh& mesh, const RenderState& state) override;
        void UseCamera(const Camera& camera) override;
        void ShowCursor(bool show) override;

        void SetViewport(const glm::ivec2& size) override;
        void Clear(const glm::vec4& color) override;
        void SetCullMode(CullMode mode) override;

        FrameBufferCreateResult CreateFrameBuffer(const FrameBufferSpecification& specification) override;
        void DestroyFrameBuffer(FrameBufferHandle framebuffer, const std::vector<TextureHandle>& colorAttachments, TextureHandle depthAttachment) override;
        void BindFrameBuffer(FrameBufferHandle framebuffer, const glm::ivec2& size) override;
        void UnbindFrameBuffer() override;
        void ClearFrameBuffer(const FrameBuffer& target, ClearFlags flags, const glm::vec4& color, float depth, int stencil) override;
        void ClearFrameBufferColor(const FrameBuffer& target, std::uint32_t attachmentIndex, const glm::vec4& color) override;
        void ClearFrameBufferDepth(const FrameBuffer& target, float depth) override;
        void ClearFrameBufferStencil(const FrameBuffer& target, int stencil) override;
        void BlitFrameBuffer(const FrameBuffer& source, FrameBuffer& target, ClearFlags mask) override;
        bool IsFrameBufferValid(FrameBufferHandle framebuffer) override;

        BufferHandle CreateBuffer(BufferType type, const void* data, size_t size, BufferUsage usage) override;
        void DestroyBuffer(BufferHandle buffer) override;
        void BindBuffer(BufferType type, BufferHandle buffer) override;
        void UpdateBuffer(BufferType type, BufferHandle buffer, size_t offset, size_t size, const void* data) override;

        VertexArrayHandle CreateVertexArray() override;
        void DestroyVertexArray(VertexArrayHandle vertexArray) override;
        void BindVertexArray(VertexArrayHandle vertexArray) override;
        void ConfigureVertexArray(VertexArrayHandle vertexArray, BufferHandle vertexBuffer, const BufferLayout& layout) override;

        TextureHandle CreateTexture2D(const TextureSpecification& specification, const void* data) override;
        void DestroyTexture(TextureHandle texture) override;
        void BindTexture(TextureHandle texture, std::uint32_t slot) override;
        void UpdateTexture2D(TextureHandle texture, const glm::ivec2& coord, const TextureSpecification& specification, const void* data) override;

        ShaderHandle CreateShader(std::string_view vertexSource, std::string_view fragmentSource) override;
        void DestroyShader(ShaderHandle shader) override;
        void BindShader(ShaderHandle shader) override;
        void SetShaderFloat(ShaderHandle shader, std::string_view name, float value) override;
        void SetShaderInt(ShaderHandle shader, std::string_view name, int value) override;
        void SetShaderVec2(ShaderHandle shader, std::string_view name, const glm::vec2& value) override;
        void SetShaderVec3(ShaderHandle shader, std::string_view name, const glm::vec3& value) override;
        void SetShaderVec4(ShaderHandle shader, std::string_view name, const glm::vec4& value) override;
        void SetShaderMat4(ShaderHandle shader, std::string_view name, const glm::mat4& value) override;

    private:
        void RequireInitialized(std::string_view method) const;
        VulkanResourceRegistry& RequireResources(std::string_view method) const;

        std::unique_ptr<VulkanInstance> m_Instance;
        std::unique_ptr<VulkanSurface> m_Surface;
        std::unique_ptr<VulkanDevice> m_Device;
        std::unique_ptr<VulkanSwapchain> m_Swapchain;
        std::unique_ptr<VulkanCommandContext> m_CommandContext;
        std::unique_ptr<VulkanResourceRegistry> m_Resources;
    };
}
