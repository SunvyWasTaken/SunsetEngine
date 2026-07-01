//
// Created by sunvy on 29/06/2026.
//

#include "Text.h"

#include "Type.h"

namespace SRmGUI
{
    void Text::Paint(FormeDatas &out)
    {
        if (!m_IsVisible)
            return;

        Widget::Paint(out);
        out.emplace_back(Forme::Text{m_DesireParameter.Position, m_DesireParameter.Size, m_Color, m_Text});
    }

    void Text::SetColor(const glm::vec4 &color)
    {
        m_Color = color;
    }

    void Text::SetText(const std::string_view &text)
    {
        m_Text = text;
    }
}
