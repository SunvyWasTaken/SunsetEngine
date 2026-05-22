//
// Created by sunvy on 07/02/2026.
//

#include "Buffers.h"

#include <complex>
#include <glad/glad.h>

namespace
{
    struct ShaderDataTypeInfo
    {
        size_t size = 0;
        int count = 0;
        int type = 0;
    };

    constexpr ShaderDataTypeInfo GetInfo(SunsetEngine::ShaderDataType type)
    {
        switch (type)
        {
            case SunsetEngine::ShaderDataType::Float:
                {
                    return {sizeof(float), 1, GL_FLOAT};
                }
            case SunsetEngine::ShaderDataType::Float2:
                {
                    return {sizeof(float) * 2, 2, GL_FLOAT};
                }
            case SunsetEngine::ShaderDataType::Float3:
                {
                    return {sizeof(float) * 3, 3, GL_FLOAT};
                }
            case SunsetEngine::ShaderDataType::Float4:
                {
                    return {sizeof(float) * 4, 4, GL_FLOAT};
                }
            case SunsetEngine::ShaderDataType::Mat3:
                {
                    return {sizeof(float) * 3 * 3, 3 * 3, GL_FLOAT};
                }
            case SunsetEngine::ShaderDataType::Mat4:
                {
                    return {sizeof(float) * 4 * 4, 4 * 4,GL_FLOAT};
                }
            case SunsetEngine::ShaderDataType::Int:
                {
                    return {sizeof(int), 1, GL_INT};
                }
            case SunsetEngine::ShaderDataType::Int2:
                {
                    return {sizeof(int) * 2, 2, GL_INT};
                }
            case SunsetEngine::ShaderDataType::Int3:
                {
                    return {sizeof(int) * 3, 3, GL_INT};
                }
            case SunsetEngine::ShaderDataType::Int4:
                {
                    return {sizeof(int) * 4, 4, GL_INT};
                }
            case SunsetEngine::ShaderDataType::UInt:
                {
                    return {sizeof(uint32_t), 1, GL_UNSIGNED_INT};
                }
            case SunsetEngine::ShaderDataType::Bool:
                {
                    return {sizeof(bool), 1, GL_BOOL};
                }
        }
        return {};
    }

    size_t CalculateStride(std::vector<SunsetEngine::BufferElement>& type)
    {
        size_t offset = 0;
        size_t stride = 0;

        for (auto& element : type)
        {
            element.offset = offset;
            offset += element.size;
            stride += element.size;
        }

        return stride;
    }
}

namespace SunsetEngine
{
    BufferElement::BufferElement(ShaderDataType type, const std::string_view& name, bool normalized)
        : name(name)
        , type(type)
        , size(GetInfo(type).size)
        , offset(0)
        , normalized(normalized)
        , divisor(0)
    {
    }

    bool BufferElement::IsInt() const
    {
        switch (type)
        {
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
        case ShaderDataType::UInt:
            return true;
        default:
            return false;
        }
    }

    int BufferElement::Count() const
    {
        return GetInfo(type).count;
    }

    int BufferElement::Type() const
    {
        return GetInfo(type).type;
    }

    BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& list)
        : m_Elements(list)
        , m_Stride(0)
    {
        m_Stride = CalculateStride(m_Elements);
    }

    const std::vector<BufferElement>& BufferLayout::operator()() const
    {
        return m_Elements;
    }

    std::vector<BufferElement>::const_iterator BufferLayout::begin() const
    {
        return m_Elements.begin();
    }

    std::vector<BufferElement>::const_iterator BufferLayout::end() const
    {
        return m_Elements.end();
    }

    bool BufferLayout::empty() const
    {
        return m_Elements.empty();
    }

    size_t BufferLayout::GetStride() const
    {
        return m_Stride;
    }

    VertexBuffer::VertexBuffer(const void* data, size_t size, size_t dataSize)
        : m_Id(0)
        , m_Layout({})
        , m_Size(size)
    {
        glGenBuffers(1, &m_Id);
        Bind();
        glBufferData(GL_ARRAY_BUFFER, size * dataSize, data, GL_STATIC_DRAW);
        Unbind();
    }
    
    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &m_Id);
    }
    
    void VertexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_Id);
    }

    void VertexBuffer::Unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBuffer::SetLayout(const std::initializer_list<BufferElement>& elements)
    {
        m_Layout = elements;
    }

    BufferLayout VertexBuffer::GetLayout() const
    {
        return m_Layout;
    }

    size_t VertexBuffer::GetSize() const
    {
        return m_Size;
    }

    IndiceBuffer::IndiceBuffer(const std::vector<uint32_t>& indices)
        : m_Id(0)
        , m_Count(indices.size())
    {
        glGenBuffers(1, &m_Id);
        Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), indices.data(), GL_STATIC_DRAW);
        Unbind();
    }

    IndiceBuffer::~IndiceBuffer()
    {
        glDeleteBuffers(1, &m_Id);
    }

    void IndiceBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
    }

    void IndiceBuffer::Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    size_t IndiceBuffer::GetCount() const
    {
        return m_Count;
    }
}
