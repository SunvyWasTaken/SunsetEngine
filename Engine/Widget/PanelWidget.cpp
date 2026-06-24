//
// Created by sunvy on 24/06/2026.
//

#include "PanelWidget.h"

namespace Sunset
{
    void PanelWidget::Update(float dt)
    {
        Widget::Update(dt);
        for (auto& widget : m_Children)
        {
            if (widget && widget->IsVisible())
                widget->Update(dt);
        }
    }

    void PanelWidget::ComputeDesiredSize()
    {
        m_DesiredSize = {0, 0};
        for (auto& widget : m_Children)
        {
            if (!widget || !widget->IsVisible())
                continue;

            widget->ComputeDesiredSize();
            m_DesiredSize = glm::max(m_DesiredSize, widget->GetDesiredSize());
        }
    }

    void PanelWidget::Arrange(const Rectangle& parentRect)
    {
        Widget::Arrange(parentRect);
        for (auto& widget : m_Children)
        {
            if (widget && widget->IsVisible())
                widget->Arrange(parentRect);
        }
    }

    void PanelWidget::Paint(UIRenderList &renderList)
    {
        if (!bIsVisible)
            return;

        Widget::Paint(renderList);
        for (auto& widget : m_Children)
        {
            if (widget && widget->IsVisible())
                widget->Paint(renderList);
        }
    }

    void PanelWidget::AddChild(const std::shared_ptr<Widget> &widget)
    {
        if (widget)
            m_Children.emplace_back(widget);
    }

    void PanelWidget::ClearChildren()
    {
        m_Children.clear();
    }

    const std::vector<std::shared_ptr<Widget>>& PanelWidget::GetChildren() const
    {
        return m_Children;
    }

    Widget * PanelWidget::HitTest(const glm::ivec2 &mouse)
    {
        if (!bIsVisible || !m_Bounds.Contains(mouse))
            return nullptr;

        for (auto it = m_Children.rbegin(); it != m_Children.rend(); ++it)
        {
            if (*it)
            {
                if (Widget* hit = (*it)->HitTest(mouse))
                    return hit;
            }
        }
        return this;
    }
} // Sunset