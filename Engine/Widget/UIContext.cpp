//
// Created by sunvy on 24/06/2026.
//

#include "UIContext.h"

namespace Sunset
{
    void UIContext::SetRoot(const std::shared_ptr<Widget> &rootWidget)
    {
        m_Root = rootWidget;
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
        rect.size.x = width;
        rect.size.y = height;

        m_Root->Arrange(rect);
    }

    void UIContext::ProcessInput()
    {
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
} // Sunset