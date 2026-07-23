//
// Created by sunvy on 16/07/2026.
//

#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace
{
    GLenum ToGLBufferTarget(const Sunset::BufferType type)
    {
        switch (type)
        {
            case Sunset::BufferType::Vertex:  return GL_ARRAY_BUFFER;
            case Sunset::BufferType::Index:   return GL_ELEMENT_ARRAY_BUFFER;
            case Sunset::BufferType::Uniform: return GL_UNIFORM_BUFFER;
        }

        return GL_ARRAY_BUFFER;
    }

    GLenum ToGLBufferUsage(const Sunset::BufferUsage usage)
    {
        switch (usage)
        {
            case Sunset::BufferUsage::Static:  return GL_STATIC_DRAW;
            case Sunset::BufferUsage::Dynamic: return GL_DYNAMIC_DRAW;
        }

        return GL_STATIC_DRAW;
    }
}

namespace Sunset::OpenGLBuffer
{
    std::uint32_t Create(const BufferType type, const void* data, const size_t size, const BufferUsage usage)
    {
        std::uint32_t buffer = 0;
        const GLenum target = ToGLBufferTarget(type);
        glGenBuffers(1, &buffer);
        glBindBuffer(target, buffer);
        glBufferData(target, size, data, ToGLBufferUsage(usage));
        glBindBuffer(target, 0);
        return buffer;
    }

    void Destroy(const std::uint32_t buffer)
    {
        glDeleteBuffers(1, &buffer);
    }

    void Bind(const BufferType type, const std::uint32_t buffer)
    {
        glBindBuffer(ToGLBufferTarget(type), buffer);
    }

    void Update(const BufferType type, const std::uint32_t buffer, const size_t offset, const size_t size, const void* data)
    {
        const GLenum target = ToGLBufferTarget(type);
        glBindBuffer(target, buffer);
        glBufferSubData(target, offset, size, data);
        glBindBuffer(target, 0);
    }
}
