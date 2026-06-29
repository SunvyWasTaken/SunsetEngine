//
// Created by sunvy on 29/06/2026.
//

#pragma once

#include "Widget.h"

namespace SRmGUI
{
    class Text : public Widget
    {
    public:
        void Paint(FormeDatas &out) override;

        void SetColor(const glm::vec4 &color);
        void SetText(const std::string_view& text);
    private:
        glm::vec4 m_Color;
        std::string m_Text;
    };
}
