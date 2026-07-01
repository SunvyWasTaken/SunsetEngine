//
// Created by sunvy on 29/06/2026.

#pragma once

#include "Widget.h"

namespace SRmGUI
{
    class Button : public Widget
    {
        using Callback = std::function<void()>;
    public:
        void Update(float dt) override;
        void Arrange(const Rect &viewportRect) override;
        void Paint(FormeDatas &out) override;
        void OnMouseMove(glm::vec2 mousePos) override;
        bool OnMouseEvent(MouseEvent::Type type, uint32_t key) override;

        void SetCallback(const Callback& callback);
        void AddChild(const std::shared_ptr<Widget> &child);
        bool m_IsHovered = false;
    private:
        WidgetPtr m_Child;
        Callback m_Callback;
    };

    template <>
    class WidgetBuilder<SRmGUI::Button> : public WidgetBuilderBase<SRmGUI::Button, WidgetBuilder<SRmGUI::Button>>
    {
    public:
        template<typename ChildT>
        WidgetBuilder& Child(const WidgetBuilder<ChildT>& child)
        {
            m_Widget->AddChild(child.ToShared());
            return *this;
        }

        WidgetBuilder& OnClicked(std::function<void()> callback)
        {
            m_Widget->SetCallback(std::move(callback));
            return *this;
        }
    };
}
