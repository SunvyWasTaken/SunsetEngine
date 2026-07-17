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
        auto& Position = m_Bounds.Position;
        auto& Size = m_Bounds.Size;

        const glm::vec2 anchorMin = glm::clamp(m_DesireParameter.AnchorMin, glm::vec2{0.0f}, m_DesireParameter.AnchorMax);
        const glm::vec2 anchorMax = glm::clamp(m_DesireParameter.AnchorMax, anchorMin, glm::vec2{1.0f});

        const glm::vec2 anchorPosition = viewportRect.Position + viewportRect.Size * anchorMin;
        const glm::vec2 anchorSize = viewportRect.Size * (anchorMax - anchorMin);

        Position = anchorPosition + m_DesireParameter.Position + m_Offset;
        Size = anchorSize + m_DesireParameter.Size;

        if (anchorMin == anchorMax)
            Size = glm::min(Size, viewportRect.Size);

        Position += glm::vec2{m_Padding.w, m_Padding.x};
        Size -= glm::vec2{m_Padding.w + m_Padding.y, m_Padding.x + m_Padding.z};
        Size = glm::max(Size, glm::vec2{0, 0});
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

    void Widget::SetAnchors(const glm::vec2 &min, const glm::vec2 &max)
    {
        SetAnchorMin(min);
        SetAnchorMax(max);
    }

    void Widget::SetAnchorMin(const glm::vec2 &min)
    {
        m_DesireParameter.AnchorMin = glm::clamp(min, {0, 0}, {1, 1});
    }

    void Widget::SetAnchorMax(const glm::vec2 &max)
    {
        m_DesireParameter.AnchorMax = glm::clamp(max, {0, 0}, {1, 1});
    }
} // SRmGUI