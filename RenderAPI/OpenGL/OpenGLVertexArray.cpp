//
// Created by Codex on 23/07/2026.
//

#include "OpenGLVertexArray.h"

#include "OpenGLBuffer.h"
#include "Render/BufferObject/Buffers.h"

#include <glad/glad.h>

namespace
{
    GLenum ToGLShaderComponentType(const Sunset::ShaderComponentType type)
    {
        switch (type)
        {
            case Sunset::ShaderComponentType::Float: return GL_FLOAT;
            case Sunset::ShaderComponentType::Int:   return GL_INT;
            case Sunset::ShaderComponentType::UInt:  return GL_UNSIGNED_INT;
            case Sunset::ShaderComponentType::Bool:  return GL_BOOL;
        }

        return GL_FLOAT;
    }
}

namespace Sunset::OpenGLVertexArray
{
    std::uint32_t Create()
    {
        std::uint32_t vertexArray = 0;
        glGenVertexArrays(1, &vertexArray);
        return vertexArray;
    }

    void Destroy(const std::uint32_t vertexArray)
    {
        glDeleteVertexArrays(1, &vertexArray);
    }

    void Bind(const std::uint32_t vertexArray)
    {
        glBindVertexArray(vertexArray);
    }

    void Configure(const std::uint32_t vertexArray, const std::uint32_t vertexBuffer, const BufferLayout& layout)
    {
        Bind(vertexArray);
        OpenGLBuffer::Bind(BufferType::Vertex, vertexBuffer);

        std::uint32_t index = 0;
        for (const BufferElement& element : layout)
        {
            glEnableVertexAttribArray(index);
            const auto offset = reinterpret_cast<const void*>(element.offset);
            const GLenum componentType = ToGLShaderComponentType(element.ComponentType());
            if (element.IsInt())
                glVertexAttribIPointer(index, element.Count(), componentType, layout.GetStride(), offset);
            else
                glVertexAttribPointer(index, element.Count(), componentType, element.normalized, layout.GetStride(), offset);

            if (element.divisor > 0)
                glVertexAttribDivisor(index, element.divisor);

            ++index;
        }

        Bind(0);
        OpenGLBuffer::Bind(BufferType::Vertex, 0);
    }
}
