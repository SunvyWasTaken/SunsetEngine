//
// Created by sunvy on 29/06/2026.
//

#include "Panel.h"

namespace SRmGUI
{
    void Panel::Update(float dt)
    {
        Widget::Update(dt);
        for (const auto& c : childs)
            c->Update(dt);
    }

    void Panel::Arrange(const Rect &viewportRect)
    {
        Widget::Arrange(viewportRect);
        for (const auto& c : childs)
            c->Arrange(c->GetDesireRect());
    }

    void Panel::Paint(FormeDatas &out)
    {
        Widget::Paint(out);
        for (const auto& c : childs)
            c->Paint(out);
    }

    void Panel::OnMouseMove(glm::vec2 mousePos)
    {
        Widget::OnMouseMove(mousePos);
        for (const auto& c : childs)
            c->OnMouseMove(mousePos);
    }

    void Panel::AddChild(const std::shared_ptr<Widget> &child)
    {
        childs.emplace_back(child);
        child->SetParent(GetSelf());
    }
}
