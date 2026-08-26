//
// Created by sunvy on 07/02/2026.
//

#pragma once

#include "Render/Core/VertexType.h"

namespace Sunset
{
    struct VertexElement final
    {
        [[nodiscard]] std::size_t Count() const;
        VertexType type;
        std::uint32_t offset = 0;
    };

    class VertexLayout final
    {
    public:
        VertexLayout(const std::initializer_list<VertexElement>& layout);

        [[nodiscard]] std::size_t GetStride() const;

        [[nodiscard]] std::vector<VertexElement>::const_iterator begin() const;

        [[nodiscard]] std::vector<VertexElement>::const_iterator end() const;

        [[nodiscard]] bool empty() const;

    private:
        std::vector<VertexElement> m_Layout;
        size_t m_Stride;
    };
} // Sunset