//
// Created by sunvy on 24/06/2026.
//

#include "PanelWidget.h"

namespace Sunset
{
    void PanelWidget::Update(float dt)
    {
        Widget::Update(dt);
        for (auto& widget : m_Childrens)
            widget->Update(dt);
    }

    void PanelWidget::Paint(UIRenderList &renderList)
    {
        Widget::Paint(renderList);
        for (auto& widget : m_Childrens)
            widget->Paint(renderList);
    }

    void PanelWidget::AddChild(const std::shared_ptr<Widget> &widget)
    {
        m_Childrens.emplace_back(widget);
    }

    Widget * PanelWidget::HitTest(const glm::ivec2 &mouse)
    {
        if (!bIsVisible || !m_Bounds.Contains(mouse))
            return nullptr;

        for (auto& widget : m_Childrens)
        {
            if (Widget* hit = widget->HitTest(mouse))
                return hit;
        }
        return nullptr;
    }
} // Sunset