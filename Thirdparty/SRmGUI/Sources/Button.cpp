//
// Created by sunvy on 29/06/2026.
//

#include "Button.h"

#include "Type.h"

namespace SRmGUI
{
    void Button::Update(float dt)
    {
        Widget::Update(dt);
        if (m_Child)
            m_Child->Update(dt);
    }

    void Button::Arrange(const Rect &viewportRect)
    {
        Widget::Arrange(viewportRect);
        if (m_Child)
            m_Child->Arrange(m_Bounds);
    }

    void Button::Paint(FormeDatas &out)
    {
        Widget::Paint(out);
        if (!m_IsVisible)
            return;

        const glm::vec4 Color = m_IsHovered ? glm::vec4(0.75f, 0.75f, 0.75f, 1.0f) : glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        out.emplace_back(Forme::Rectangle{m_Bounds.Position, m_Bounds.Size, Color});

        if (m_Child)
            m_Child->Paint(out);
    }

    void Button::OnMouseMove(glm::vec2 mousePos)
    {
        Widget::OnMouseMove(mousePos);
        if (!m_IsVisible)
            return;

        if (mousePos.x >= m_Bounds.Position.x && mousePos.x <= m_Bounds.Position.x + m_Bounds.Size.x &&
            mousePos.y >= m_Bounds.Position.y && mousePos.y <= m_Bounds.Position.y + m_Bounds.Size.y)
        {
            m_IsHovered = true;
        }
        else
            m_IsHovered = false;
    }

    bool Button::OnMouseEvent(uint8_t type, uint8_t key)
    {
        Widget::OnMouseEvent(type, key);
        if (!m_IsVisible)
            return false;

        if (m_IsHovered)
        {
            if (key == 0)
            {
                if (m_Callback)
                    m_Callback();
                return true;
            }
        }
        return false;
    }

    void Button::SetCallback(const Callback& callback)
    {
        m_Callback = callback;
    }

    void Button::AddChild(const std::shared_ptr<Widget> &child)
    {
        m_Child = child;
    }
}
