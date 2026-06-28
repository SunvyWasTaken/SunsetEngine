//
// Created by sunvy on 24/06/2026.
//

#include "Widget.h"

#include "Render/UI/UIRenderList.h"

namespace Sunset
{
    bool Rectangle::Contains(const glm::ivec2 &pos) const
    {
        return pos.x >= position.x && pos.x < position.x + size.x &&
               pos.y >= position.y && pos.y < position.y + size.y;
    }

    void Widget::Update(float dt)
    {
    }

    void Widget::ComputeDesiredSize()
    {
    }

    void Widget::Arrange(const Rectangle &parentRect)
    {
        m_Bounds.size = glm::min(m_DesiredSize, parentRect.size);
        m_Bounds.size = glm::max(m_Bounds.size, glm::ivec2{0, 0});

        const glm::ivec2 maxPosition = parentRect.position + glm::max(parentRect.size - m_Bounds.size, glm::ivec2{0, 0});
        m_Bounds.position = glm::clamp(m_Bounds.position, parentRect.position, maxPosition);
    }

    void Widget::Paint(UIRenderList& renderList)
    {
    }

    Widget * Widget::HitTest(const glm::ivec2 &mouse)
    {
        if (bIsVisible && m_Bounds.Contains(mouse))
            return this;

        return nullptr;
    }

    void Widget::SetVisible(bool visible)
    {
        bIsVisible = visible;
    }

    bool Widget::IsVisible() const
    {
        return bIsVisible;
    }

    void Widget::SetDesiredSize(const glm::ivec2& desiredSize)
    {
        m_DesiredSize = glm::max(desiredSize, glm::ivec2{0, 0});
    }

    const glm::ivec2& Widget::GetDesiredSize() const
    {
        return m_DesiredSize;
    }

    const Rectangle& Widget::GetBounds() const
    {
        return m_Bounds;
    }

    void Widget::OnHovered(bool hovered)
    {
    }
} // Sunset