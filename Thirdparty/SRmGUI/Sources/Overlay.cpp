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
        const glm::vec2 windowSize = GetContext().GetWindowSize();

        Forme::Rectangle rect;
        rect.Position = {0, 0};
        rect.Size = windowSize * 0.5f;
        rect.Color = {1.0f, 1.0f, 1.0f, 1.f};
        out.emplace_back(rect);
    }
} // SRmGUI