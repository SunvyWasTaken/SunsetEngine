//
// Created by sunvy on 29/06/2026.
//

#include "Panel.h"

namespace SRmGUI
{
    void Panel::Update(float dt)
    {
        if (!m_IsVisible)
            return;
        Widget::Update(dt);
        for (const auto& c : childs)
            c->Update(dt);
    }

    void Panel::Arrange(const Rect &viewportRect)
    {
        Widget::Arrange(viewportRect);

        if (!m_IsVisible)
            return;

        for (const auto& c : childs)
            c->Arrange(m_Bounds);
    }

    void Panel::Paint(FormeDatas &out)
    {
        if (!m_IsVisible)
            return;

        Widget::Paint(out);
        for (const auto& c : childs)
            c->Paint(out);
    }

    void Panel::OnMouseMove(glm::vec2 mousePos)
    {
        if (!m_IsVisible)
            return;

        Widget::OnMouseMove(mousePos);
        for (const auto& c : childs)
            c->OnMouseMove(mousePos);
    }

    bool Panel::OnMouseEvent(MouseEvent::Type type, uint32_t key)
    {
        if (!m_IsVisible)
            return false;

        Widget::OnMouseEvent(type, key);

        return std::ranges::any_of(childs, [&](const auto& c){ return c->OnMouseEvent(type, key); });
    }

    WidgetPtr Panel::HitTest(const glm::vec2 &mousePos)
    {
        for (auto c = childs.end(); c != childs.begin(); )
            if (auto hitWidget = (*--c)->HitTest(mousePos))
                return hitWidget;

        return Widget::HitTest(mousePos);
    }

    void Panel::AddChild(const std::shared_ptr<Widget> &child)
    {
        childs.emplace_back(child);
        child->SetParent(GetSelf());
    }
}
