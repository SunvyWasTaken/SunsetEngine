//
// Created by Codex on 23/07/2026.
//

#include "VulkanResourceRegistry.h"

#include "Render/FrameBufferSpecification.h"

#include <cstring>
#include <limits>

namespace
{
    bool IsDepthFormat(const Sunset::FrameBufferTextureFormat format)
    {
        switch (format)
        {
            case Sunset::FrameBufferTextureFormat::Depth24Stencil8:
            case Sunset::FrameBufferTextureFormat::Depth32F:
                return true;
            default:
                return false;
        }
    }

    Sunset::TextureFormat ToTextureFormat(const Sunset::FrameBufferTextureFormat format)
    {
        switch (format)
        {
            case Sunset::FrameBufferTextureFormat::R32F:
            case Sunset::FrameBufferTextureFormat::Depth24Stencil8:
            case Sunset::FrameBufferTextureFormat::Depth32F:
                return Sunset::TextureFormat::Red;
            case Sunset::FrameBufferTextureFormat::RGBA8:
            case Sunset::FrameBufferTextureFormat::RGBA16F:
                return Sunset::TextureFormat::RGBA;
            case Sunset::FrameBufferTextureFormat::None:
                return Sunset::TextureFormat::RGB;
        }

        return Sunset::TextureFormat::RGB;
    }

    size_t BytesPerPixel(const Sunset::TextureFormat format)
    {
        switch (format)
        {
            case Sunset::TextureFormat::Red: return 1;
            case Sunset::TextureFormat::RGB: return 3;
            case Sunset::TextureFormat::RGBA: return 4;
        }

        return 4;
    }

    size_t TextureByteSize(const Sunset::TextureSpecification& specification)
    {
        if (specification.width <= 0 || specification.height <= 0)
            return 0;

        return static_cast<size_t>(specification.width) * static_cast<size_t>(specification.height) * BytesPerPixel(specification.format);
    }
}

namespace Sunset
{
    FrameBufferCreateResult VulkanResourceRegistry::CreateFrameBuffer(const FrameBufferSpecification& specification)
    {
        FrameBufferResource resource;
        resource.specification = specification;

        for (const FrameBufferTextureSpecification& attachment : specification.attachments)
        {
            if (attachment.format == FrameBufferTextureFormat::None)
                continue;

            const TextureSpecification textureSpecification{
                .width = specification.width,
                .height = specification.height,
                .format = ToTextureFormat(attachment.format)
            };

            const TextureHandle texture = CreateTexture2D(textureSpecification, nullptr);
            if (IsDepthFormat(attachment.format))
                resource.depthAttachment = texture;
            else
                resource.colorAttachments.emplace_back(texture);
        }

        const FrameBufferHandle handle = AllocateHandle<FrameBufferHandle>();
        FrameBufferCreateResult result{
            .id = handle,
            .colorAttachments = resource.colorAttachments,
            .depthAttachment = resource.depthAttachment
        };

        m_FrameBuffers.emplace(handle.id, std::move(resource));
        return result;
    }

    void VulkanResourceRegistry::DestroyFrameBuffer(const FrameBufferHandle framebuffer)
    {
        const auto it = m_FrameBuffers.find(framebuffer.id);
        if (it == m_FrameBuffers.end())
            return;

        for (const TextureHandle texture : it->second.colorAttachments)
            DestroyTexture(texture);

        DestroyTexture(it->second.depthAttachment);
        m_FrameBuffers.erase(it);
    }

    bool VulkanResourceRegistry::IsFrameBufferValid(const FrameBufferHandle framebuffer) const
    {
        return m_FrameBuffers.contains(framebuffer.id);
    }

    BufferHandle VulkanResourceRegistry::CreateBuffer(const BufferType type, const void* data, const size_t size, const BufferUsage usage)
    {
        BufferResource resource{
            .type = type,
            .usage = usage,
            .bytes = std::vector<std::byte>(size)
        };

        if (data != nullptr && size > 0)
            std::memcpy(resource.bytes.data(), data, size);

        const BufferHandle handle = AllocateHandle<BufferHandle>();
        m_Buffers.emplace(handle.id, std::move(resource));
        return handle;
    }

    void VulkanResourceRegistry::DestroyBuffer(const BufferHandle buffer)
    {
        m_Buffers.erase(buffer.id);
    }

    void VulkanResourceRegistry::UpdateBuffer(const BufferHandle buffer, const size_t offset, const size_t size, const void* data)
    {
        auto it = m_Buffers.find(buffer.id);
        if (it == m_Buffers.end() || data == nullptr || size == 0)
            return;

        if (offset > std::numeric_limits<size_t>::max() - size)
            return;

        const size_t end = offset + size;
        if (end > it->second.bytes.size())
            it->second.bytes.resize(end);

        std::memcpy(it->second.bytes.data() + offset, data, size);
    }

    VertexArrayHandle VulkanResourceRegistry::CreateVertexArray()
    {
        const VertexArrayHandle handle = AllocateHandle<VertexArrayHandle>();
        m_VertexArrays.emplace(handle.id, VertexArrayResource{});
        return handle;
    }

    void VulkanResourceRegistry::DestroyVertexArray(const VertexArrayHandle vertexArray)
    {
        m_VertexArrays.erase(vertexArray.id);
    }

    void VulkanResourceRegistry::ConfigureVertexArray(const VertexArrayHandle vertexArray, const BufferHandle vertexBuffer, const BufferLayout&)
    {
        auto it = m_VertexArrays.find(vertexArray.id);
        if (it == m_VertexArrays.end())
            return;

        it->second.vertexBuffer = vertexBuffer;
    }

    TextureHandle VulkanResourceRegistry::CreateTexture2D(const TextureSpecification& specification, const void* data)
    {
        const size_t byteSize = TextureByteSize(specification);
        TextureResource resource{
            .specification = specification,
            .bytes = std::vector<std::byte>(byteSize)
        };

        if (data != nullptr && byteSize > 0)
            std::memcpy(resource.bytes.data(), data, byteSize);

        const TextureHandle handle = AllocateHandle<TextureHandle>();
        m_Textures.emplace(handle.id, std::move(resource));
        return handle;
    }

    void VulkanResourceRegistry::DestroyTexture(const TextureHandle texture)
    {
        m_Textures.erase(texture.id);
    }

    void VulkanResourceRegistry::UpdateTexture2D(const TextureHandle texture, const TextureSpecification& specification, const void* data)
    {
        auto it = m_Textures.find(texture.id);
        if (it == m_Textures.end())
            return;

        const size_t byteSize = TextureByteSize(specification);
        it->second.specification = specification;
        it->second.bytes.resize(byteSize);

        if (data != nullptr && byteSize > 0)
            std::memcpy(it->second.bytes.data(), data, byteSize);
    }

    ShaderHandle VulkanResourceRegistry::CreateShader(const std::string_view vertexSource, const std::string_view fragmentSource)
    {
        const ShaderHandle handle = AllocateHandle<ShaderHandle>();
        m_Shaders.emplace(handle.id, ShaderResource{
            .vertexSource = std::string(vertexSource),
            .fragmentSource = std::string(fragmentSource)
        });
        return handle;
    }

    void VulkanResourceRegistry::DestroyShader(const ShaderHandle shader)
    {
        m_Shaders.erase(shader.id);
    }
}
