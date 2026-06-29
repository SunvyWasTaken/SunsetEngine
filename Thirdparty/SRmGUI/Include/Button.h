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
        bool OnMouseEvent(uint8_t type, uint8_t key) override;
        void SetCallback(const Callback& callback);
        bool IsHovered = false;
    private:
        WidgetPtr child;
        Callback m_Callback;
    };
}
