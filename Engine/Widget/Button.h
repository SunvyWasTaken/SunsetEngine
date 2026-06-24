//
// Created by sunvy on 24/06/2026.
//

#pragma once

#include "LeafWidget.h"

namespace Sunset
{
    class Button : public LeafWidget
    {
    public:
        using Callback = std::function<void()>;

        explicit Button(Callback onClicked);

        void Paint(UIRenderList& renderList) override;
        Widget* HitTest(const glm::ivec2& mouse) override;

        void SetOnClicked(Callback onClicked);
        void SetColors(const glm::vec4& normal, const glm::vec4& hovered, const glm::vec4& pressed);
        void SetRadius(int radius);
        void SetHovered(bool hovered);
        void SetPressed(bool pressed);
        void Click();

        [[nodiscard]] bool IsHovered() const;
        [[nodiscard]] bool IsPressed() const;

    private:
        Callback m_OnClicked;
        glm::vec4 m_NormalColor {0.18f, 0.20f, 0.24f, 1.0f};
        glm::vec4 m_HoveredColor {0.26f, 0.29f, 0.34f, 1.0f};
        glm::vec4 m_PressedColor {0.10f, 0.12f, 0.16f, 1.0f};
        int m_Radius = 4;
        bool m_IsHovered = false;
        bool m_IsPressed = false;
    };
} // Sunset