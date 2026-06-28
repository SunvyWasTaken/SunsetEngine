//
// Created by sunvy on 28/06/2026.
//

#include "Context.h"

namespace SRmGUI
{
    void Context::SetRoot(const std::shared_ptr<Widget> &root)
    {
        m_Root = root;
    }

    void Context::Update(float dt)
    {
    }

    void Context::HandleEvent()
    {
    }

    void Context::SetLayout(const Rect &viewportRect)
    {
        m_ViewportRect = viewportRect;
    }

    void Context::Paint()
    {
    }
} // SRmGUI