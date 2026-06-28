//
// Created by sunvy on 28/06/2026.
//

#include "Context.h"

#include "Type.h"
#include "Widget.h"

namespace SRmGUI
{
    Context::Context()
    {
    }

    Context::~Context()
    {
    }

    void Context::SetRoot(const WidgetPtr&root)
    {
        m_Root = root;
    }

    void Context::Update(float dt)
    {
        if (m_Root)
            m_Root->Update(dt);
    }

    void Context::HandleEvent()
    {
    }

    void Context::SetLayout(const Rect&viewportRect)
    {
        m_ViewportRect = viewportRect;
    }

    void Context::Paint()
    {
        m_FormeDatas.clear();
        if (m_Root)
            m_Root->Paint(m_FormeDatas);
    }

    const FormeDatas & Context::GetData()
    {
        return m_FormeDatas;
    }

    glm::vec2 Context::GetWindowSize() const
    {
        return m_ViewportRect.Size;
    }
} // SRmGUI