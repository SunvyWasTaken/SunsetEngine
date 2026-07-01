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

    void Context::HandleEvent(const MouseEvent& mouseEvent)
    {
        if (!m_Root)
            return;

        const WidgetPtr widget = m_Root->HitTest(mouseEvent.position);

        if (!widget)
            return;

        if (mouseEvent.type != MouseEvent::Type::None && mouseEvent.key == 0)
        {
            if (mouseEvent.type == MouseEvent::Type::Press)
            {
                m_DragState.IsMouseDown = true;
                m_DragState.PressedWidget = widget;
                m_DragState.PressMousePos = mouseEvent.position;
                m_DragState.CurrentMousePos = mouseEvent.position;
            }
            else if (mouseEvent.type == MouseEvent::Type::Release)
            {
                if (m_DragState.TargetWidget)
                    m_DragState.TargetWidget->OnDrop(m_DragState.DragPayload);

                m_DragState = {};
            }
        }

        if (m_DragState.IsMouseDown && !m_DragState.IsDragging)
        {
            float distance = glm::distance(glm::vec2{mouseEvent.position.x, mouseEvent.position.y}, glm::vec2{m_DragState.PressMousePos.x, m_DragState.PressMousePos.y});
            if (distance > 2.f && m_DragState.PressedWidget)
            {
                DragDropPayload dragPayload;

                if (m_DragState.PressedWidget->OnDragDetected(dragPayload))
                {
                    m_DragState.IsDragging = true;
                    m_DragState.SourceWidget = widget;
                    m_DragState.DragPayload = dragPayload;
                }
            }
        }

        if (m_DragState.IsDragging)
        {
            if (widget && widget->CanAcceptDrag(m_DragState.DragPayload))
                m_DragState.TargetWidget = widget;
            else
                m_DragState.TargetWidget = nullptr;
        }

        if (mouseEvent.type != MouseEvent::Type::None)
            widget->OnMouseEvent(mouseEvent.type, mouseEvent.key);
        else
            widget->OnMouseMove(mouseEvent.position);
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