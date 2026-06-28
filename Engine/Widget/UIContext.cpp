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

    void UIContext::AddWidget(const std::shared_ptr<Widget> &rootWidget)
    {
        m_Roots.emplace_back(rootWidget);
    }

    void UIContext::Update(float dt)
    {
        for (auto& root : m_Roots)
            if (root)
                root->Update(dt);
    }

    void UIContext::PrePass()
    {
        for (auto& root : m_Roots)
        if (root)
            root->ComputeDesiredSize();
    }

    void UIContext::Arrange(const int width, const int height)
    {
        if (m_Roots.empty())
            return;

        Rectangle rect;
        rect.position.x = 0;
        rect.position.y = 0;
        rect.size.x = width;
        rect.size.y = height;

        for (auto& root : m_Roots)
            if (root)
                root->Arrange(rect);
    }

    void UIContext::ProcessInput()
    {
    }

    void UIContext::ProcessMouseMove(const glm::ivec2& mousePosition)
    {
        Widget* hoveredWidget = nullptr;
        for (auto& root : m_Roots)
            if (root)
                if (Widget* hit = root->HitTest(mousePosition))
                    hoveredWidget = hit;

        if (m_HoveredWidget && m_HoveredWidget != hoveredWidget)
        {
            m_HoveredWidget = hoveredWidget;
            m_HoveredWidget->OnHovered(false);
        }
    }

    bool UIContext::ProcessMouseButton(unsigned int button, Event::Action action, const glm::ivec2& mousePosition)
    {
        if (button != 0)
            return false;

        ProcessMouseMove(mousePosition);
        if (action == Event::Action::Press)
        {
            m_PressedButton = dynamic_cast<Button*>(m_HoveredWidget);
            if (m_PressedButton)
            {
                m_PressedButton->SetPressed(true);
                return true;
            }
        }
        else if (action == Event::Action::Release && m_PressedButton)
        {
            Button* pressedButton = m_PressedButton;
            pressedButton->SetPressed(false);
            m_PressedButton = nullptr;

            if (pressedButton == dynamic_cast<Button*>(m_HoveredWidget))
                pressedButton->Click();
            return true;
        }

        return false;
    }

    void UIContext::Paint()
    {
        m_RenderList.Clear();
        for (auto& root : m_Roots)
            if (root)
                root->Paint(m_RenderList);
    }

    UIRenderList & UIContext::GetRenderList()
    {
        return m_RenderList;
    }
} // Sunset
