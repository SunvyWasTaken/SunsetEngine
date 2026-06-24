//
// Created by sunvy on 24/06/2026.
//

#include "UIRenderList.h"

namespace Sunset
{
    void UIRenderList::Clear()
    {
        m_Rectangles.clear();
    }

    void UIRenderList::AddRectangle(const glm::ivec2& position, const glm::ivec2& size, const glm::vec4& color, int radius)
    {
        if (size.x <= 0 || size.y <= 0)
            return;

        m_Rectangles.push_back({position, size, color, radius});
    }

    bool UIRenderList::IsEmpty() const
    {
        return m_Rectangles.empty();
    }
} // Sunset