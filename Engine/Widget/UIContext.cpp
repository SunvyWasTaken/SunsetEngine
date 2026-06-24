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

    void UIContext::Arrange(float width, float height)
    {
        if (!m_Roots.empty())
            return;

        Rectangle rect;
        rect.position.x = 0;
        rect.position.y = 0;
        rect.size.x = static_cast<int>(width);
        rect.size.y = static_cast<int>(height);

        for (auto& root : m_Roots)
            root->Arrange(rect);
    }

    void UIContext::ProcessInput()
    {
    }

    void UIContext::ProcessMouseMove(const glm::ivec2& mousePosition)
    {
        for (auto& root : m_Roots)
            if (root)
                root->HitTest(mousePosition);
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