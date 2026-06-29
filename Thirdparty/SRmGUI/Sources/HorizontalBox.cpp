//
// Created by sunvy on 29/06/2026.
//

#include "HorizontalBox.h"

#include "Type.h"

namespace SRmGUI
{
    void HorizontalBox::Arrange(const Rect &viewportRect)
    {
        // Panel::Arrange(viewportRect);

        Rect rect = m_DesireParameter;

        const auto nbrChild = childs.size();
        const auto objSize = (rect.Size.y - padding * nbrChild) / nbrChild;

        const auto start = rect.Position.y - rect.Size.y / 2;

        uint32_t i = 0;
        for (const auto& c : childs)
        {
            c->Arrange({{start + (objSize + padding + padding) * i++, rect.Position.y}, {rect.Size.x, objSize}});
        }
    }

    void HorizontalBox::Paint(FormeDatas &out)
    {
        Panel::Paint(out);

        // Forme::Rectangle rect;
        // rect.Position = m_DesireParameter.Position;
        // rect.Size = m_DesireParameter.Size;
        // rect.Color = {1.0f, 1.0f, 1.0f, 1.f};
        // out.emplace_back(rect);
    }
}
