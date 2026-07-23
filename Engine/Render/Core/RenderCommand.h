//
// Created by sunvy on 05/01/2026.
//

#pragma once

#include "Render/Core/RenderHandle.h"

#include <cstdint>
#include <memory>
#include <string_view>
#include <vector>

namespace Sunset
{
    struct BufferLayout;
    struct FrameBufferCreateResult;
    struct FrameBufferSpecification;
    struct RenderState;
    struct TextureSpecification;
    enum class ClearFlags : std::uint8_t;
    enum class CullMode;
    enum class BufferType;
    enum class BufferUsage;
    class RenderAPI;
    class Mesh;
    class Camera;
    class Drawable;
    class FrameBuffer;
    class Texture;
    class Textures;

    struct RenderCommand
    {
        static void SetRenderAPI(std::unique_ptr<RenderAPI> api);
        static bool HasRenderAPI();

        static void BeginFrame();
        static void EndFrame();
        static void Shutdown();

        static void BeginTarget(FrameBuffer& target, const glm::vec4& clearColor = glm::vec4(0.0f));
        static void EndTarget();
        static void Flush();

        static void Submit(const Drawable& mesh, const glm::mat4& model);
        static void DrawMesh(const Mesh& mesh, const RenderState& state);
        static void UseCamera(const Camera& camera);
        static void ShowCursor(bool show);
        static void SetViewport(const glm::ivec2& viewport);
        static void SetCullMode(CullMode mode);

        static FrameBufferCreateResult CreateFrameBuffer(const FrameBufferSpecification& specification);
        static void DestroyFrameBuffer(FrameBufferHandle framebuffer, const std::vector<TextureHandle>& colorAttachments, TextureHandle depthAttachment);
        static void BindFrameBuffer(FrameBufferHandle framebuffer, const glm::ivec2& size);
        static void UnbindFrameBuffer();
        static void ClearFrameBuffer(const FrameBuffer& target, ClearFlags flags, const glm::vec4& color, float depth, int stencil);
        static void ClearFrameBufferColor(const FrameBuffer& target, std::uint32_t attachmentIndex, const glm::vec4& color);
        static void ClearFrameBufferDepth(const FrameBuffer& target, float depth);
        static void ClearFrameBufferStencil(const FrameBuffer& target, int stencil);
        static void BlitFrameBuffer(const FrameBuffer& source, FrameBuffer& target, ClearFlags mask);
        static bool IsFrameBufferValid(FrameBufferHandle framebuffer);

        static BufferHandle CreateBuffer(BufferType type, const void* data, size_t size, BufferUsage usage);
        static void DestroyBuffer(BufferHandle buffer);
        static void BindBuffer(BufferType type, BufferHandle buffer);
        static void UpdateBuffer(BufferType type, BufferHandle buffer, size_t offset, size_t size, const void* data);

        static VertexArrayHandle CreateVertexArray();
        static void DestroyVertexArray(VertexArrayHandle vertexArray);
        static void BindVertexArray(VertexArrayHandle vertexArray);
        static void ConfigureVertexArray(VertexArrayHandle vertexArray, BufferHandle vertexBuffer, const BufferLayout& layout);

        static TextureHandle CreateTexture2D(const TextureSpecification& specification, const void* data = nullptr);
        static void DestroyTexture(TextureHandle texture);
        static void BindTexture(TextureHandle texture, std::uint32_t slot = 0);
        static void BindTexture(const Texture& texture, std::uint32_t slot = 0);
        static void BindTexture(const Textures& texture, std::uint32_t slot = 0);
        static void UpdateTexture2D(TextureHandle texture, const glm::ivec2& coord, const TextureSpecification& specification, const void* data);

        static ShaderHandle CreateShader(std::string_view vertexSource, std::string_view fragmentSource);
        static void DestroyShader(ShaderHandle shader);
        static void BindShader(ShaderHandle shader);
        static void SetShaderFloat(ShaderHandle shader, std::string_view name, float value);
        static void SetShaderInt(ShaderHandle shader, std::string_view name, int value);
        static void SetShaderVec2(ShaderHandle shader, std::string_view name, const glm::vec2& value);
        static void SetShaderVec3(ShaderHandle shader, std::string_view name, const glm::vec3& value);
        static void SetShaderVec4(ShaderHandle shader, std::string_view name, const glm::vec4& value);
        static void SetShaderMat4(ShaderHandle shader, std::string_view name, const glm::mat4& value);
    };
}
