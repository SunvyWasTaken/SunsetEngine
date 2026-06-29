//
// Created by sunvy on 29/06/2026.
//

#include "VerticalBox.h"

namespace SRmGUI
{
    void VerticalBox::Arrange(const Rect &viewportRect)
    {
        Widget::Arrange(viewportRect);
        const auto NbrElement = childs.size();

        const auto objSize = (m_DesireParameter.Size.y - (padding * NbrElement)) / NbrElement;
        const auto start = m_DesireParameter.Position.y - m_DesireParameter.Size.y / 2;

        int i = 0;
        for (const auto& c : childs)
        {
            c->Arrange({{m_DesireParameter.Position.x, start + (objSize + padding + padding) * i++}, {m_DesireParameter.Size.x, objSize}});
        }
    }
}
