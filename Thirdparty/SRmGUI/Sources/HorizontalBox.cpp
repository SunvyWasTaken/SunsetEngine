//
// Created by sunvy on 29/06/2026.
//

#include "HorizontalBox.h"

#include "Type.h"

namespace SRmGUI
{
    void HorizontalBox::Arrange(const Rect &viewportRect)
    {
        Widget::Arrange(viewportRect);

        auto&[Position, Size] = m_DesireParameter;

        const auto nbrChild = childs.size();
        const auto objSize = Size.x / nbrChild;

        uint32_t i = 0;
        for (const auto& c : childs)
        {
            c->Arrange({{Position.x + objSize * i++, Position.y}, {objSize, Size.y}});
        }
    }

    void HorizontalBox::Paint(FormeDatas &out)
    {
        // Forme::Rectangle rect;
        // rect.Position = m_DesireParameter.Position;
        // rect.Size = m_DesireParameter.Size;
        // rect.Color = {1.0f, 1.0f, 1.0f, 1.f};
        // out.emplace_back(rect);

        Panel::Paint(out);
    }
}
