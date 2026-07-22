//
// Created by sunvy on 15/12/2025.
//

#pragma once

namespace Sunset
{
    struct BufferLayout;
    struct RenderState;
    class Camera;
    class Drawable;
    class FrameBuffer;
    class Mesh;

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

        virtual std::uint32_t CreateBuffer(BufferType type, const void* data, size_t size, BufferUsage usage) = 0;
        virtual void DestroyBuffer(std::uint32_t buffer) = 0;
        virtual void BindBuffer(BufferType type, std::uint32_t buffer) = 0;
        virtual void UpdateBuffer(BufferType type, std::uint32_t buffer, size_t offset, size_t size, const void* data) = 0;

        virtual std::uint32_t CreateVertexArray() = 0;
        virtual void DestroyVertexArray(std::uint32_t vertexArray) = 0;
        virtual void BindVertexArray(std::uint32_t vertexArray) = 0;
        virtual void ConfigureVertexArray(std::uint32_t vertexArray, std::uint32_t vertexBuffer, const BufferLayout& layout) = 0;

        virtual std::uint32_t CreateTexture2D(const TextureSpecification& specification, const void* data) = 0;
        virtual void DestroyTexture(std::uint32_t texture) = 0;
        virtual void BindTexture(std::uint32_t texture, std::uint32_t slot) = 0;
        virtual void UpdateTexture2D(std::uint32_t texture, const glm::ivec2& coord, const TextureSpecification& specification, const void* data) = 0;
    };
}
