//
// Created by sunvy on 28/06/2026.
//

#include "Widget.h"

namespace SRmGUI
{
    Widget::Widget()
        : m_Parent()
        , m_DesireParameter()
        , m_IsVisible(true)
    {
    }

    void Widget::SetParent(const std::shared_ptr<Widget> &parent)
    {
        m_Parent = parent;
    }

    std::shared_ptr<Widget> Widget::GetSelf()
    {
        return shared_from_this();
    }

    void Widget::Update(float dt)
    {
    }

    void Widget::Arrange(const Rect &viewportRect)
    {
        m_DesireParameter.Position = viewportRect.Position;
        m_DesireParameter.Size = viewportRect.Size;
    }

    void Widget::Paint(FormeDatas &out)
    {
    }

    void Widget::SetPosition(const glm::vec2 &position)
    {
        m_DesireParameter.Position = position;
    }

    void Widget::SetSize(const glm::vec2 &size)
    {
        m_DesireParameter.Size = size;
    }
} // SRmGUI