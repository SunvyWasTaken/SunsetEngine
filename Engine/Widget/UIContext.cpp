//
// Created by sunvy on 24/06/2026.
//

#include "UIContext.h"

#include "Button.h"
#include "Widget.h"

namespace Sunset
{
    UIContext::~UIContext()
    {
    }

    void UIContext::SetRoot(const std::shared_ptr<Widget> &rootWidget)
    {
        ClearButtonState();
    }

    std::shared_ptr<Widget> UIContext::GetRoot() const
    {
        return m_Root;
    }

    void UIContext::Update(float dt)
    {
        if (m_Root)
            m_Root->Update(dt);
    }

    void UIContext::PrePass()
    {
        if (m_Root)
            m_Root->ComputeDesiredSize();
    }

    void UIContext::Arrange(float width, float height)
    {
        if (!m_Root)
            return;

        Rectangle rect;
        rect.position.x = 0;
        rect.position.y = 0;
        rect.size.x = static_cast<int>(width);
        rect.size.y = static_cast<int>(height);

        m_Root->Arrange(rect);
    }

    void UIContext::ProcessInput()
    {
    }

    void UIContext::ProcessMouseMove(const glm::ivec2& mousePosition)
    {
        Widget* previousHoveredWidget = m_HoveredWidget;
        m_HoveredWidget = m_Root ? m_Root->HitTest(mousePosition) : nullptr;

        if (previousHoveredWidget && previousHoveredWidget != m_HoveredWidget)
        {
            if (Button* previousButton = dynamic_cast<Button*>(previousHoveredWidget))
                previousButton->SetHovered(false);
        }
    }

    bool UIContext::ProcessMouseButton(const glm::ivec2& mousePosition, bool pressed)
    {
        ProcessMouseMove(mousePosition);

        if (pressed)
        {
            m_PressedButton = dynamic_cast<Button*>(m_HoveredWidget);
            if (m_PressedButton)
            {
                m_PressedButton->SetPressed(true);
                return true;
            }
            return false;
        }

        Button* releasedButton = dynamic_cast<Button*>(m_HoveredWidget);
        const bool clicked = m_PressedButton && releasedButton == m_PressedButton;
        if (m_PressedButton)
            m_PressedButton->SetPressed(false);

        if (clicked)
            releasedButton->Click();

        m_PressedButton = nullptr;
        return clicked;
    }

    void UIContext::Paint()
    {
        m_RenderList.Clear();
        if (m_Root)
            m_Root->Paint(m_RenderList);
    }

    UIRenderList & UIContext::GetRenderList()
    {
        return m_RenderList;
    }

    void UIContext::ClearButtonState()
    {
        if (m_PressedButton)
            m_PressedButton->SetPressed(false);

        m_HoveredWidget = nullptr;
        m_PressedButton = nullptr;
    }
} // Sunset