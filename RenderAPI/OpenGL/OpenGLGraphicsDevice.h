//
// Created by sunvy on 16/07/2026.
//

#pragma once

#include "Render/Core/RenderAPI.h"
#include "Render/Core/RenderType.h"
#include "OpenGLDrawQueue.h"
#include "OpenGLContext.h"

namespace Sunset
{
    class OpenGLGraphicsDevice final : public RenderAPI
    {
    public:
        ~OpenGLGraphicsDevice() override;

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
        void DestroyFrameBuffer(std::uint32_t framebuffer, const std::vector<std::uint32_t>& colorAttachments, std::uint32_t depthAttachment) override;
        void BindFrameBuffer(std::uint32_t framebuffer, const glm::ivec2& size) override;
        void UnbindFrameBuffer() override;
        void ClearFrameBuffer(const FrameBuffer& target, ClearFlags flags, const glm::vec4& color, float depth, int stencil) override;
        void ClearFrameBufferColor(const FrameBuffer& target, std::uint32_t attachmentIndex, const glm::vec4& color) override;
        void ClearFrameBufferDepth(const FrameBuffer& target, float depth) override;
        void ClearFrameBufferStencil(const FrameBuffer& target, int stencil) override;
        void BlitFrameBuffer(const FrameBuffer& source, FrameBuffer& target, ClearFlags mask) override;
        bool IsFrameBufferValid(std::uint32_t framebuffer) override;

        std::uint32_t CreateBuffer(BufferType type, const void* data, size_t size, BufferUsage usage) override;
        void DestroyBuffer(std::uint32_t buffer) override;
        void BindBuffer(BufferType type, std::uint32_t buffer) override;
        void UpdateBuffer(BufferType type, std::uint32_t buffer, size_t offset, size_t size, const void* data) override;

        std::uint32_t CreateVertexArray() override;
        void DestroyVertexArray(std::uint32_t vertexArray) override;
        void BindVertexArray(std::uint32_t vertexArray) override;
        void ConfigureVertexArray(std::uint32_t vertexArray, std::uint32_t vertexBuffer, const BufferLayout& layout) override;

        std::uint32_t CreateTexture2D(const TextureSpecification& specification, const void* data) override;
        void DestroyTexture(std::uint32_t texture) override;
        void BindTexture(std::uint32_t texture, std::uint32_t slot) override;
        void UpdateTexture2D(std::uint32_t texture, const glm::ivec2& coord, const TextureSpecification& specification, const void* data) override;

        std::uint32_t CreateShader(std::string_view vertexSource, std::string_view fragmentSource) override;
        void DestroyShader(std::uint32_t shader) override;
        void BindShader(std::uint32_t shader) override;
        void SetShaderFloat(std::uint32_t shader, std::string_view name, float value) override;
        void SetShaderInt(std::uint32_t shader, std::string_view name, int value) override;
        void SetShaderVec2(std::uint32_t shader, std::string_view name, const glm::vec2& value) override;
        void SetShaderVec3(std::uint32_t shader, std::string_view name, const glm::vec3& value) override;
        void SetShaderVec4(std::uint32_t shader, std::string_view name, const glm::vec4& value) override;
        void SetShaderMat4(std::uint32_t shader, std::string_view name, const glm::mat4& value) override;

    private:
        void ResetFrameState() const;
        void ApplyState(const RenderState& state) const;
        void DrawBoundMesh(std::uint32_t vertexCount, const RenderState& state) const;
        void FlushDrawCommands();

        std::unique_ptr<OpenGLContext> m_Context;
        OpenGLDrawQueue m_DrawQueue;
    };
} // Sunset
