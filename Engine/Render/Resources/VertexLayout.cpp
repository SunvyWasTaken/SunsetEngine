//
// Created by sunvy on 07/02/2026.
//

#include "VertexLayout.h"

namespace
{
    std::size_t GetSize(const Sunset::VertexType type)
    {
        switch (type)
        {
            case Sunset::VertexType::Float1: return sizeof(float);
            case Sunset::VertexType::Float2: return sizeof(float) * 2;
            case Sunset::VertexType::Float3: return sizeof(float) * 3;
            case Sunset::VertexType::Float4: return sizeof(float) * 4;

            case Sunset::VertexType::Int1: return sizeof(int);
            case Sunset::VertexType::Int2: return sizeof(int) * 2;
            case Sunset::VertexType::Int3: return sizeof(int) * 3;
            case Sunset::VertexType::Int4: return sizeof(int) * 4;

            case Sunset::VertexType::Mat3: return sizeof(glm::mat3);
            case Sunset::VertexType::Mat4: return sizeof(glm::mat4);
            case Sunset::VertexType::UInt: return sizeof(std::uint32_t);
        }
        return 0;
    }

    int GetCount(const Sunset::VertexType type)
    {
        switch (type)
        {
            case Sunset::VertexType::Float1:    return 1;
            case Sunset::VertexType::Float2:    return 2;
            case Sunset::VertexType::Float3:    return 3;
            case Sunset::VertexType::Float4:    return 4;
            case Sunset::VertexType::Int1:      return 1;
            case Sunset::VertexType::Int2:      return 2;
            case Sunset::VertexType::Int3:      return 3;
            case Sunset::VertexType::Int4:      return 4;
            case Sunset::VertexType::Mat3:      return 3 * 3;
            case Sunset::VertexType::Mat4:      return 4 * 4;
            case Sunset::VertexType::UInt:      return 1;
        }
        return 0;
    }

    size_t CalculateStride(std::vector<Sunset::VertexElement>& type)
    {
        size_t offset = 0;
        size_t stride = 0;

        for (auto& element : type)
        {
            element.offset = offset;
            offset += GetSize(element.type);
            stride += GetSize(element.type);
        }

        return stride;
    }
}

namespace Sunset
{
    std::size_t VertexElement::Count() const
    {
        return ::GetCount(type);
    }

    VertexLayout::VertexLayout(const std::initializer_list<VertexElement> &layout)
        : m_Layout(layout)
        , m_Stride(CalculateStride(m_Layout))
    {
    }

    std::size_t VertexLayout::GetStride() const
    {
        return m_Layout.size();
    }

    std::vector<VertexElement>::const_iterator VertexLayout::begin() const
    {
        return m_Layout.begin();
    }

    std::vector<VertexElement>::const_iterator VertexLayout::end() const
    {
        return m_Layout.end();
    }

    bool VertexLayout::empty() const
    {
        return m_Layout.empty();
    }
} // Sunset