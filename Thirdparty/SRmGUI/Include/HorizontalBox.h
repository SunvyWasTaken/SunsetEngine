//
// Created by sunvy on 29/06/2026.
//

#pragma once

#include "Panel.h"

namespace SRmGUI
{
    class HorizontalBox : public Panel
    {
    public:
        void Arrange(const Rect &viewportRect) override;
        void Paint(FormeDatas &out) override;
    };
}
