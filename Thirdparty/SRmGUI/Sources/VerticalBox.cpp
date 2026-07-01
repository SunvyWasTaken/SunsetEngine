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

        const auto objSize = m_Bounds.Size.y / NbrElement;
        const auto start = m_Bounds.Position.y - m_Bounds.Size.y / 2;

        int i = 0;
        for (const auto& c : childs)
        {
            c->Arrange({{m_Bounds.Position.x, start + objSize * i++}, {m_Bounds.Size.x, objSize}});
        }
    }
}
