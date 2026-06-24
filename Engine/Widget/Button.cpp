//
// Created by sunvy on 24/06/2026.
//

#include "Button.h"

#include "Render/UI/UIRenderList.h"

namespace Sunset
{
    Button::Button(Callback onClicked)
    {
        m_OnClicked = std::move(onClicked);
    }

    void Button::Paint(UIRenderList& renderList)
    {
        if (!bIsVisible)
            return;

        const glm::vec4& color = m_IsPressed ? m_PressedColor : (m_IsHovered ? m_HoveredColor : m_NormalColor);
        renderList.AddRectangle(m_Bounds.position, m_Bounds.size, color, m_Radius);
    }

    Widget* Button::HitTest(const glm::ivec2& mouse)
    {
        m_IsHovered = bIsVisible && m_Bounds.Contains(mouse);
        return m_IsHovered ? this : nullptr;
    }

    void Button::SetOnClicked(Callback onClicked)
    {
        m_OnClicked = std::move(onClicked);
    }

    void Button::SetColors(const glm::vec4& normal, const glm::vec4& hovered, const glm::vec4& pressed)
    {
        m_NormalColor = normal;
        m_HoveredColor = hovered;
        m_PressedColor = pressed;
    }

    void Button::SetRadius(int radius)
    {
        m_Radius = std::max(0, radius);
    }

    void Button::SetHovered(bool hovered)
    {
        m_IsHovered = hovered;
    }

    void Button::SetPressed(bool pressed)
    {
        m_IsPressed = pressed;
    }

    void Button::Click()
    {
        if (m_OnClicked)
            m_OnClicked();
    }

    bool Button::IsHovered() const
    {
        return m_IsHovered;
    }

    bool Button::IsPressed() const
    {
        return m_IsPressed;
    }
} // Sunset