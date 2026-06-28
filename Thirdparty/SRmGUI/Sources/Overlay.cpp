//
// Created by sunvy on 28/06/2026.
//

#include "Overlay.h"

#include "Type.h"

namespace SRmGUI
{
    void Overlay::Paint(FormeDatas &out)
    {
        Widget::Paint(out);
        out.emplace_back(Forme::Rectangle{{{0,0}, {0.5, 0.5}}, 0});
    }
} // SRmGUI