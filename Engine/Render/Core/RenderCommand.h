//
// Created by sunvy on 05/01/2026.
//

#pragma once

namespace Sunset
{
    struct BufferLayout;
    struct RenderState;
    struct TextureSpecification;
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

        static std::uint32_t CreateBuffer(BufferType type, const void* data, size_t size, BufferUsage usage);
        static void DestroyBuffer(std::uint32_t buffer);
        static void BindBuffer(BufferType type, std::uint32_t buffer);
        static void UpdateBuffer(BufferType type, std::uint32_t buffer, size_t offset, size_t size, const void* data);

        static std::uint32_t CreateVertexArray();
        static void DestroyVertexArray(std::uint32_t vertexArray);
        static void BindVertexArray(std::uint32_t vertexArray);
        static void ConfigureVertexArray(std::uint32_t vertexArray, std::uint32_t vertexBuffer, const BufferLayout& layout);

        static std::uint32_t CreateTexture2D(const TextureSpecification& specification, const void* data = nullptr);
        static void DestroyTexture(std::uint32_t texture);
        static void BindTexture(std::uint32_t texture, std::uint32_t slot = 0);
        static void BindTexture(const Texture& texture, std::uint32_t slot = 0);
        static void BindTexture(const Textures& texture, std::uint32_t slot = 0);
        static void UpdateTexture2D(std::uint32_t texture, const glm::ivec2& coord, const TextureSpecification& specification, const void* data);
    };
}
