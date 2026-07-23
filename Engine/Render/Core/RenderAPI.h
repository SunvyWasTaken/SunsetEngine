//
// Created by sunvy on 15/12/2025.
//

#pragma once

#include "Render/FrameBufferSpecification.h"
#include "Render/Core/RenderHandle.h"
#include "Render/Core/RenderType.h"

#include <glm/glm.hpp>

#include <cstdint>
#include <string_view>
#include <vector>

namespace Sunset
{
    struct BufferLayout;
    struct RenderState;
    class Camera;
    class Drawable;
    class FrameBuffer;
    class Mesh;

    struct FrameBufferCreateResult
    {
        FrameBufferHandle id;
        std::vector<TextureHandle> colorAttachments;
        TextureHandle depthAttachment;
    };

    enum class BufferType
    {
        Vertex,
        Index,
        Uniform
    };

    enum class BufferUsage
    {
        Static,
        Dynamic
    };

    enum class ShaderComponentType
    {
        Float,
        Int,
        UInt,
        Bool
    };

    enum class TextureFormat
    {
        Red,
        RGB,
        RGBA
    };

    struct TextureSpecification
    {
        int width = 0;
        int height = 0;
        TextureFormat format = TextureFormat::RGB;
    };

    class RenderAPI
    {
    public:
        virtual ~RenderAPI() = default;

        virtual void Init() = 0;
        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;

        virtual void BeginTarget(FrameBuffer& target, const glm::vec4& clearColor) = 0;
        virtual void EndTarget() = 0;
        virtual void Flush() = 0;

        virtual void Submit(const Drawable& drawable, const glm::mat4& model) = 0;
        virtual void DrawMesh(const Mesh& mesh, const RenderState& state) = 0;
        virtual void UseCamera(const Camera& camera) = 0;
        virtual void ShowCursor(bool show) = 0;

        virtual void SetViewport(const glm::ivec2& size) = 0;
        virtual void Clear(const glm::vec4& color) = 0;
        virtual void SetCullMode(CullMode mode) = 0;

        virtual FrameBufferCreateResult CreateFrameBuffer(const FrameBufferSpecification& specification) = 0;
        virtual void DestroyFrameBuffer(FrameBufferHandle framebuffer, const std::vector<TextureHandle>& colorAttachments, TextureHandle depthAttachment) = 0;
        virtual void BindFrameBuffer(FrameBufferHandle framebuffer, const glm::ivec2& size) = 0;
        virtual void UnbindFrameBuffer() = 0;
        virtual void ClearFrameBuffer(const FrameBuffer& target, ClearFlags flags, const glm::vec4& color, float depth, int stencil) = 0;
        virtual void ClearFrameBufferColor(const FrameBuffer& target, std::uint32_t attachmentIndex, const glm::vec4& color) = 0;
        virtual void ClearFrameBufferDepth(const FrameBuffer& target, float depth) = 0;
        virtual void ClearFrameBufferStencil(const FrameBuffer& target, int stencil) = 0;
        virtual void BlitFrameBuffer(const FrameBuffer& source, FrameBuffer& target, ClearFlags mask) = 0;
        virtual bool IsFrameBufferValid(FrameBufferHandle framebuffer) = 0;

        virtual BufferHandle CreateBuffer(BufferType type, const void* data, size_t size, BufferUsage usage) = 0;
        virtual void DestroyBuffer(BufferHandle buffer) = 0;
        virtual void BindBuffer(BufferType type, BufferHandle buffer) = 0;
        virtual void UpdateBuffer(BufferType type, BufferHandle buffer, size_t offset, size_t size, const void* data) = 0;

        virtual VertexArrayHandle CreateVertexArray() = 0;
        virtual void DestroyVertexArray(VertexArrayHandle vertexArray) = 0;
        virtual void BindVertexArray(VertexArrayHandle vertexArray) = 0;
        virtual void ConfigureVertexArray(VertexArrayHandle vertexArray, BufferHandle vertexBuffer, const BufferLayout& layout) = 0;

        virtual TextureHandle CreateTexture2D(const TextureSpecification& specification, const void* data) = 0;
        virtual void DestroyTexture(TextureHandle texture) = 0;
        virtual void BindTexture(TextureHandle texture, std::uint32_t slot) = 0;
        virtual void UpdateTexture2D(TextureHandle texture, const glm::ivec2& coord, const TextureSpecification& specification, const void* data) = 0;

        virtual ShaderHandle CreateShader(std::string_view vertexSource, std::string_view fragmentSource) = 0;
        virtual void DestroyShader(ShaderHandle shader) = 0;
        virtual void BindShader(ShaderHandle shader) = 0;
        virtual void SetShaderFloat(ShaderHandle shader, std::string_view name, float value) = 0;
        virtual void SetShaderInt(ShaderHandle shader, std::string_view name, int value) = 0;
        virtual void SetShaderVec2(ShaderHandle shader, std::string_view name, const glm::vec2& value) = 0;
        virtual void SetShaderVec3(ShaderHandle shader, std::string_view name, const glm::vec3& value) = 0;
        virtual void SetShaderVec4(ShaderHandle shader, std::string_view name, const glm::vec4& value) = 0;
        virtual void SetShaderMat4(ShaderHandle shader, std::string_view name, const glm::mat4& value) = 0;
    };
}
