//
// Created by sunvy on 30/06/2026.
//

#include "GridPanel.h"

namespace SRmGUI
{
    void GridPanel::Arrange(const Rect &viewportRect)
    {
        Widget::Arrange(viewportRect);

        int cellSizeX = m_Bounds.Size.x / m_NbrGrid.x;
        int cellSizeY = m_Bounds.Size.y / m_NbrGrid.y;

        int i = 0;
        for (const auto& c : childs)
        {
            Rect rect;
            rect.Position = m_Bounds.Position + glm::vec2{cellSizeX * (i % m_NbrGrid.x), cellSizeY * (i / m_NbrGrid.x)};
            rect.Size = {cellSizeX, cellSizeY};
            c->Arrange(rect);
            ++i;
        }
    }

    void GridPanel::SetColumn(int32_t nbr)
    {
        m_NbrGrid.x = nbr;
    }

    void GridPanel::SetRow(int32_t nbr)
    {
        m_NbrGrid.y = nbr;
    }
}
