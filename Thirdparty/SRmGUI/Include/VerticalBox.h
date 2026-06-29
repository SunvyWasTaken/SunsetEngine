//
// Created by sunvy on 29/06/2026.
//

#pragma once
#include "Panel.h"

namespace SRmGUI
{
    class VerticalBox : public Panel
    {
    public:
        void Arrange(const Rect &viewportRect) override;
    private:
        int padding = 1;
    };
}
