//
// Created by sunvy on 28/06/2026.
//

#pragma once

#include "Widget.h"

namespace SRmGUI
{
    class Image : public Widget
    {
    public:
        void Update(float dt) override;
        void Arrange(const Rect& viewportRect) override;
        void Paint(FormeDatas& out) override;
    };
} // SRmGUI