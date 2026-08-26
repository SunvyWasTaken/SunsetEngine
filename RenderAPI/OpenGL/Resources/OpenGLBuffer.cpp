//
// Created by sunvy on 26/08/2026.
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

    std::string_view ToText(const Sunset::BufferType type)
    {
        switch (type)
        {
            case Sunset::BufferType::Vertex:  return "Vertex";
            case Sunset::BufferType::Index:   return "Index";
            case Sunset::BufferType::Uniform: return "Uniform";
        }
        return "";
    }
}

namespace Sunset
{
    OpenGLBuffer::OpenGLBuffer(const BufferType &type)
        : m_Size(0)
        , m_Type(type)
    {
        glGenBuffers(1, &id);
        LOG("OpenGL", trace, "OpenGL buffer {}, id : {} created", ToText(m_Type), id);
    }

    OpenGLBuffer::~OpenGLBuffer()
    {
        LOG("OpenGL", trace, "OpenGL buffer {}, id : {} Destroy", ToText(m_Type), id);
        glDeleteBuffers(1, &id);
    }

    void OpenGLBuffer::Bind()
    {
        glBindBuffer(ToGLBufferTarget(m_Type), id);
    }

    void OpenGLBuffer::SetData(const void *data, const std::size_t typeSize, const std::size_t size)
    {
        m_Size = size;
        const GLenum target = ToGLBufferTarget(m_Type);
        glBindBuffer(target, id);
        glBufferData(target, typeSize * size, data, GL_STATIC_DRAW);
        glBindBuffer(target, 0);
    }

    std::size_t OpenGLBuffer::Count() const
    {
        return m_Size;
    }
} // Sunset