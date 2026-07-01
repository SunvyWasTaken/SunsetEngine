//
// Created by sunvy on 28/06/2026.
//

#include "Overlay.h"

#include "SRmGUI.h"
#include "../Include/Type.h"

namespace SRmGUI
{
    void Overlay::Arrange(const Rect &viewportRect)
    {
        Widget::Arrange(viewportRect);
        for (const auto& c : childs)
            c->Arrange(m_Bounds);
    }

    void Overlay::Paint(FormeDatas &out)
    {
        if (!m_IsVisible)
            return;

        Widget::Paint(out);
        for (const auto& c : childs)
            c->Paint(out);

        // Forme::Rectangle rect;
        // rect.Position = m_DesireParameter.Position;
        // rect.Size = m_DesireParameter.Size;
        // rect.Color = {1.0f, 1.0f, 1.0f, 1.f};
        // out.emplace_back(rect);
    }
} // SRmGUI