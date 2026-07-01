//
// Created by sunvy on 29/06/2026.
//

#pragma once

#include "Widget.h"

namespace SRmGUI
{
    class Panel : public Widget
    {
    public:
        void Update(float dt) override;
        void Arrange(const Rect &viewportRect) override;
        void Paint(FormeDatas &out) override;
        void OnMouseMove(glm::vec2 mousePos) override;
        bool OnMouseEvent(MouseEvent::Type type, uint32_t key) override;
        WidgetPtr HitTest(const glm::vec2& mousePos) override;

        virtual void AddChild(const std::shared_ptr<Widget>& child);
    protected:
        std::vector<WidgetPtr> childs;
    };
}
