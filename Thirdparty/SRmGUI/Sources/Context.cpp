//
// Created by sunvy on 28/06/2026.
//

#include "Context.h"

#include "../Include/Type.h"
#include "../Include/Widget.h"

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

    void Context::HandleMousePos(const glm::vec2 &mousePos)
    {
        if (m_Root)
            m_Root->OnMouseMove(mousePos);
    }

    bool Context::HandleMouseEvent(const uint8_t type, const uint8_t key)
    {
        if (m_Root)
            return m_Root->OnMouseEvent(type, key);

        return false;
    }

    void Context::Arrange(const Rect& viewportRect)
    {
        m_ViewportRect = viewportRect;

        if (m_Root)
            m_Root->Arrange(m_ViewportRect);
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