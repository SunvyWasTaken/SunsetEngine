//
// Created by sunvy on 28/06/2026.
//

#include "Widget.h"

#include "Type.h"

namespace SRmGUI
{
    Widget::Widget()
        : m_Parent()
        , m_DesireParameter()
        , m_IsVisible(true)
        , m_Padding(0, 0, 0, 0)
        , m_Offset(0, 0)
    {
    }

    void Widget::SetParent(const std::shared_ptr<Widget> &parent)
    {
        m_Parent = parent;
    }

    std::shared_ptr<Widget> Widget::GetSelf()
    {
        return shared_from_this();
    }

    void Widget::Update(float dt)
    {
    }

    void Widget::Arrange(const Rect &viewportRect)
    {
        auto&[Position, Size] = m_Bounds;

        Size = glm::min(m_DesireParameter.Size, viewportRect.Size);
        Size = glm::max(Size, glm::vec2{0, 0});

        Position = viewportRect.Position + m_Offset;

        Position += glm::ivec2{m_Padding.w, m_Padding.x};
        Size -= glm::ivec2{m_Padding.w + m_Padding.y, m_Padding.x + m_Padding.z};
    }

    void Widget::Paint(FormeDatas &out)
    {
    }

    void Widget::OnMouseMove(glm::vec2 mousePos)
    {
    }

    bool Widget::OnMouseEvent(MouseEvent::Type type, uint32_t key)
    {
        return false;
    }

    bool Widget::OnDragDetected(DragDropPayload& payload)
    {
        if (m_OnDragDetected)
            return m_OnDragDetected(payload);
        return false;
    }

    bool Widget::CanAcceptDrag(const DragDropPayload &dragPayload)
    {
        if (m_CanAcceptDrag)
            return m_CanAcceptDrag(dragPayload);
        return false;
    }

    void Widget::OnDrop(const DragDropPayload &dragPayload)
    {
        if (m_OnDrop)
            return m_OnDrop(dragPayload);
    }

    WidgetPtr Widget::HitTest(const glm::vec2 &mousePos)
    {
        if (!m_IsVisible)
            return nullptr;

        if (m_Bounds.Position.x <= mousePos.x && mousePos.x <= m_Bounds.Position.x + m_Bounds.Size.x &&
            m_Bounds.Position.y <= mousePos.y && mousePos.y <= m_Bounds.Position.y + m_Bounds.Size.y)
            return shared_from_this();
        return nullptr;
    }

    Rect Widget::GetDesireRect() const
    {
        return m_DesireParameter;
    }

    glm::vec2 Widget::GetPosition() const
    {
        return m_Bounds.Position;
    }

    void Widget::SetPosition(const glm::vec2 &position)
    {
        m_DesireParameter.Position = position;
    }

    void Widget::SetSize(const glm::vec2 &size)
    {
        m_DesireParameter.Size = size;
    }

    void Widget::SetVisibility(const bool visible)
    {
        m_IsVisible = visible;
    }

    void Widget::SetPadding(const glm::vec4 &padding)
    {
        m_Padding = padding;
    }

    void Widget::SetOffset(const glm::vec2 &offset)
    {
        m_Offset = offset;
    }
} // SRmGUI