//
// Created by Codex on 23/07/2026.
//

#include "OpenGLGraphicsDevice.h"

#include "OpenGLBuffer.h"
#include "OpenGLTexture.h"
#include "OpenGLVertexArray.h"

namespace Sunset
{
    BufferHandle OpenGLGraphicsDevice::CreateBuffer(const BufferType type, const void* data, const size_t size, const BufferUsage usage)
    {
        return BufferHandle{OpenGLBuffer::Create(type, data, size, usage)};
    }

    void OpenGLGraphicsDevice::DestroyBuffer(const BufferHandle buffer)
    {
        OpenGLBuffer::Destroy(buffer.id);
    }

    void OpenGLGraphicsDevice::BindBuffer(const BufferType type, const BufferHandle buffer)
    {
        OpenGLBuffer::Bind(type, buffer.id);
    }

    void OpenGLGraphicsDevice::UpdateBuffer(const BufferType type, const BufferHandle buffer, const size_t offset, const size_t size, const void* data)
    {
        OpenGLBuffer::Update(type, buffer.id, offset, size, data);
    }

    VertexArrayHandle OpenGLGraphicsDevice::CreateVertexArray()
    {
        return VertexArrayHandle{OpenGLVertexArray::Create()};
    }

    void OpenGLGraphicsDevice::DestroyVertexArray(const VertexArrayHandle vertexArray)
    {
        OpenGLVertexArray::Destroy(vertexArray.id);
    }

    void OpenGLGraphicsDevice::BindVertexArray(const VertexArrayHandle vertexArray)
    {
        OpenGLVertexArray::Bind(vertexArray.id);
    }

    void OpenGLGraphicsDevice::ConfigureVertexArray(const VertexArrayHandle vertexArray, const BufferHandle vertexBuffer, const BufferLayout& layout)
    {
        OpenGLVertexArray::Configure(vertexArray.id, vertexBuffer.id, layout);
    }

    TextureHandle OpenGLGraphicsDevice::CreateTexture2D(const TextureSpecification& specification, const void* data)
    {
        return TextureHandle{OpenGLTexture::Create2D(specification, data)};
    }

    void OpenGLGraphicsDevice::DestroyTexture(const TextureHandle texture)
    {
        OpenGLTexture::Destroy(texture.id);
    }

    void OpenGLGraphicsDevice::BindTexture(const TextureHandle texture, const std::uint32_t slot)
    {
        OpenGLTexture::Bind(texture.id, slot);
    }

    void OpenGLGraphicsDevice::UpdateTexture2D(const TextureHandle texture, const glm::ivec2& coord, const TextureSpecification& specification, const void* data)
    {
        OpenGLTexture::Update2D(texture.id, coord, specification, data);
    }
}
