//
// Created by sunvy on 24/06/2026.
//

#include "Widget.h"

#include "Render/UI/UIRenderList.h"

namespace Sunset
{
    bool Rectangle::Contains(const glm::ivec2 &pos) const
    {
        if (pos.x >= position.x && pos.x <= position.x + size.x &&
            pos.y >= position.y && pos.y <= position.y + size.y)
            return true;

        return false;
    }

    void Widget::Update(float dt)
    {
    }

    void Widget::ComputeDesiredSize()
    {
    }

    void Widget::Arrange(const Rectangle &parentRect)
    {
    }

    void Widget::Paint(UIRenderList& renderList)
    {
    }

    Widget * Widget::HitTest(const glm::ivec2 &mouse)
    {
        if (m_Bounds.Contains(mouse))
            return this;

        return nullptr;
    }
} // Sunset