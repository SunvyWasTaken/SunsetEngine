//
// Created by Codex on 23/07/2026.
//

#include "OpenGLGraphicsDevice.h"

#include "OpenGLBuffer.h"
#include "OpenGLTexture.h"
#include "OpenGLVertexArray.h"

namespace Sunset
{
    std::uint32_t OpenGLGraphicsDevice::CreateBuffer(const BufferType type, const void* data, const size_t size, const BufferUsage usage)
    {
        return OpenGLBuffer::Create(type, data, size, usage);
    }

    void OpenGLGraphicsDevice::DestroyBuffer(const std::uint32_t buffer)
    {
        OpenGLBuffer::Destroy(buffer);
    }

    void OpenGLGraphicsDevice::BindBuffer(const BufferType type, const std::uint32_t buffer)
    {
        OpenGLBuffer::Bind(type, buffer);
    }

    void OpenGLGraphicsDevice::UpdateBuffer(const BufferType type, const std::uint32_t buffer, const size_t offset, const size_t size, const void* data)
    {
        OpenGLBuffer::Update(type, buffer, offset, size, data);
    }

    std::uint32_t OpenGLGraphicsDevice::CreateVertexArray()
    {
        return OpenGLVertexArray::Create();
    }

    void OpenGLGraphicsDevice::DestroyVertexArray(const std::uint32_t vertexArray)
    {
        OpenGLVertexArray::Destroy(vertexArray);
    }

    void OpenGLGraphicsDevice::BindVertexArray(const std::uint32_t vertexArray)
    {
        OpenGLVertexArray::Bind(vertexArray);
    }

    void OpenGLGraphicsDevice::ConfigureVertexArray(const std::uint32_t vertexArray, const std::uint32_t vertexBuffer, const BufferLayout& layout)
    {
        OpenGLVertexArray::Configure(vertexArray, vertexBuffer, layout);
    }

    std::uint32_t OpenGLGraphicsDevice::CreateTexture2D(const TextureSpecification& specification, const void* data)
    {
        return OpenGLTexture::Create2D(specification, data);
    }

    void OpenGLGraphicsDevice::DestroyTexture(const std::uint32_t texture)
    {
        OpenGLTexture::Destroy(texture);
    }

    void OpenGLGraphicsDevice::BindTexture(const std::uint32_t texture, const std::uint32_t slot)
    {
        OpenGLTexture::Bind(texture, slot);
    }

    void OpenGLGraphicsDevice::UpdateTexture2D(const std::uint32_t texture, const glm::ivec2& coord, const TextureSpecification& specification, const void* data)
    {
        OpenGLTexture::Update2D(texture, coord, specification, data);
    }
}
