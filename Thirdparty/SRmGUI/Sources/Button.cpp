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
        if (child)
            child->Update(dt);
    }

    void Button::Arrange(const Rect &viewportRect)
    {
        Widget::Arrange(viewportRect);
        if (child)
            child->Arrange(m_DesireParameter);
    }

    void Button::Paint(FormeDatas &out)
    {
        Widget::Paint(out);
        const glm::vec4 Color = IsHovered ? glm::vec4(0.75f, 0.75f, 0.75f, 1.0f) : glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        out.emplace_back(Forme::Rectangle{m_DesireParameter.Position, m_DesireParameter.Size, Color});
        if (child)
            child->Paint(out);
    }

    void Button::OnMouseMove(glm::vec2 mousePos)
    {
        Widget::OnMouseMove(mousePos);
        if (mousePos.x >= m_DesireParameter.Position.x && mousePos.x <= m_DesireParameter.Position.x + m_DesireParameter.Size.x &&
            mousePos.y >= m_DesireParameter.Position.y && mousePos.y <= m_DesireParameter.Position.y + m_DesireParameter.Size.y)
        {
            IsHovered = true;
        }
        else
            IsHovered = false;
    }
}
