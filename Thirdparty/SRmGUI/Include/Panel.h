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
        bool OnMouseEvent(uint8_t type, uint8_t key) override;

        virtual void AddChild(const std::shared_ptr<Widget>& child);
    protected:
        std::vector<WidgetPtr> childs;
    };
}
