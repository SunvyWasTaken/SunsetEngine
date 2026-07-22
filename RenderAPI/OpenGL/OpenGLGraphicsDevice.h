//
// Created by sunvy on 16/07/2026.
//

#pragma once

#include "Render/Core/RenderAPI.h"
#include "Render/Core/RenderType.h"

namespace Sunset
{
    class Material;

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

    private:
        struct FrameData
        {
            glm::vec3 position = glm::vec3(0.0f);
            glm::mat4 view = glm::mat4(1.0f);
            glm::mat4 projection = glm::mat4(1.0f);
        };

        struct DrawCommand
        {
            uint32_t vertexArray = 0;
            uint32_t indexCount = 0;
            std::shared_ptr<Material> material;
            glm::mat4 model = glm::mat4(1.0f);
            RenderState state;
        };

        void ResetFrameState() const;
        void ApplyState(const RenderState& state) const;
        void DrawBoundMesh(std::uint32_t vertexCount, const RenderState& state) const;
        void SortDrawCommands();
        void FlushDrawCommands();

        FrameData m_FrameData;
        std::vector<DrawCommand> m_DrawCommands;
    };
} // Sunset
