//
// Created by sunvy on 29/06/2026.

#pragma once

#include "Widget.h"

namespace SRmGUI
{
    class Button : public Widget
    {
    public:
        void Update(float dt) override;
        void Arrange(const Rect &viewportRect) override;
        void Paint(FormeDatas &out) override;
        void OnMouseMove(glm::vec2 mousePos) override;
        bool IsHovered = false;
    private:
        WidgetPtr child;
    };
}
