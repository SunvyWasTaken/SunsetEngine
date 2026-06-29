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

    template<>
    class WidgetBuilder<SRmGUI::Text>
    : public WidgetBuilderBase<SRmGUI::Text, WidgetBuilder<SRmGUI::Text>>
    {
    public:
        WidgetBuilder()
        {
            m_Widget = std::make_shared<SRmGUI::Text>();
        }

        WidgetBuilder& Text(const std::string& text)
        {
            m_Widget->SetText(std::move(text));
            return *this;
        }

        WidgetBuilder& Color(glm::vec4 color)
        {
            m_Widget->SetColor(color);
            return *this;
        }
    };
}
