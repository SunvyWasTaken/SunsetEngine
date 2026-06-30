//
// Created by sunvy on 28/06/2026.
//

#pragma once

#include "Panel.h"

namespace SRmGUI
{
    class Overlay : public Panel
    {
    public:
        ~Overlay() override = default;
        void Arrange(const Rect &viewportRect) override;
        void Paint(FormeDatas &out) override;
    };
} // SRmGUi