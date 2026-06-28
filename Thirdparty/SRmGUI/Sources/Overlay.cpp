//
// Created by sunvy on 28/06/2026.
//

#include "Overlay.h"

#include "SRmGUI.h"
#include "Type.h"

namespace SRmGUI
{
    void Overlay::Paint(FormeDatas &out)
    {
        Widget::Paint(out);

        Forme::Rectangle rect;
        rect.Position = m_DesireParameter.Position;
        rect.Size = m_DesireParameter.Size;
        rect.Color = {1.0f, 1.0f, 1.0f, 1.f};
        out.emplace_back(rect);
    }
} // SRmGUI