//
// Created by sunvy on 26/08/2026.
//

#include "OpenGLMesh.h"

#include <glad/glad.h>

#include "Render/Resources/Buffer.h"

namespace
{
    GLenum ToOpenGLType(const Sunset::VertexType type)
    {
        switch (type)
        {
            case Sunset::VertexType::Float1:    return GL_FLOAT;
            case Sunset::VertexType::Float2:    return GL_FLOAT;
            case Sunset::VertexType::Float3:    return GL_FLOAT;
            case Sunset::VertexType::Float4:    return GL_FLOAT;
            case Sunset::VertexType::Int1:      return GL_INT;
            case Sunset::VertexType::Int2:      return GL_INT;
            case Sunset::VertexType::Int3:      return GL_INT;
            case Sunset::VertexType::Int4:      return GL_INT;
            case Sunset::VertexType::Mat3:      return GL_FLOAT;
            case Sunset::VertexType::Mat4:      return GL_FLOAT;
            case Sunset::VertexType::UInt:      return GL_UNSIGNED_INT;
        }
        return GL_FLOAT;
    }
}

namespace Sunset
{
    OpenGLMesh::OpenGLMesh(const std::shared_ptr<Buffer> &vertexBuffer, const std::shared_ptr<Buffer> &indexBuffer,
        const VertexLayout &vertexLayout)
            : Mesh(vertexBuffer, indexBuffer, vertexLayout)
    {
        glGenVertexArrays(1, &id);
        glBindVertexArray(id);

        m_VertexBuffer->Bind();

        std::uint32_t index = 0;
        for (const auto &element : m_VertexLayout)
        {
            glEnableVertexAttribArray(index);
            const auto offset = reinterpret_cast<const void*>(element.offset);
            const GLenum type = ToOpenGLType(element.type);
            if (type == GL_INT)
                glVertexAttribIPointer(index, element.Count(), type, m_VertexLayout.GetStride(), offset);
            else
                glVertexAttribPointer(index, element.Count(), type, false, m_VertexLayout.GetStride(), offset);

            ++index;
        }

        glBindVertexArray(0);

        LOG("OpenGL", trace, "OpenGL mesh {} created", id)
    }

    OpenGLMesh::~OpenGLMesh()
    {
        LOG("OpenGL", trace, "OpenGL mesh {} destroyed", id)
        glDeleteVertexArrays(1, &id);
    }

    void OpenGLMesh::Bind()
    {
        glBindVertexArray(id);
    }
} // Sunset