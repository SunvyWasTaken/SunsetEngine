//
// Created by Codex on 23/07/2026.
//

#pragma once

#include "Render/Core/RenderAPI.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace Sunset
{
    class VulkanResourceRegistry
    {
    public:
        [[nodiscard]] FrameBufferCreateResult CreateFrameBuffer(const FrameBufferSpecification& specification);
        void DestroyFrameBuffer(FrameBufferHandle framebuffer);
        [[nodiscard]] bool IsFrameBufferValid(FrameBufferHandle framebuffer) const;

        [[nodiscard]] BufferHandle CreateBuffer(BufferType type, const void* data, size_t size, BufferUsage usage);
        void DestroyBuffer(BufferHandle buffer);
        void UpdateBuffer(BufferHandle buffer, size_t offset, size_t size, const void* data);

        [[nodiscard]] VertexArrayHandle CreateVertexArray();
        void DestroyVertexArray(VertexArrayHandle vertexArray);
        void ConfigureVertexArray(VertexArrayHandle vertexArray, BufferHandle vertexBuffer, const BufferLayout& layout);

        [[nodiscard]] TextureHandle CreateTexture2D(const TextureSpecification& specification, const void* data);
        void DestroyTexture(TextureHandle texture);
        void UpdateTexture2D(TextureHandle texture, const TextureSpecification& specification, const void* data);

        [[nodiscard]] ShaderHandle CreateShader(std::string_view vertexSource, std::string_view fragmentSource);
        void DestroyShader(ShaderHandle shader);

    private:
        struct BufferResource
        {
            BufferType type = BufferType::Vertex;
            BufferUsage usage = BufferUsage::Static;
            std::vector<std::byte> bytes;
        };

        struct VertexArrayResource
        {
            BufferHandle vertexBuffer;
        };

        struct TextureResource
        {
            TextureSpecification specification;
            std::vector<std::byte> bytes;
        };

        struct ShaderResource
        {
            std::string vertexSource;
            std::string fragmentSource;
        };

        struct FrameBufferResource
        {
            FrameBufferSpecification specification;
            std::vector<TextureHandle> colorAttachments;
            TextureHandle depthAttachment;
        };

        template <typename Handle>
        [[nodiscard]] Handle AllocateHandle()
        {
            return Handle{m_NextHandle++};
        }

        std::uint32_t m_NextHandle = 1;
        std::unordered_map<std::uint32_t, BufferResource> m_Buffers;
        std::unordered_map<std::uint32_t, VertexArrayResource> m_VertexArrays;
        std::unordered_map<std::uint32_t, TextureResource> m_Textures;
        std::unordered_map<std::uint32_t, ShaderResource> m_Shaders;
        std::unordered_map<std::uint32_t, FrameBufferResource> m_FrameBuffers;
    };
}
