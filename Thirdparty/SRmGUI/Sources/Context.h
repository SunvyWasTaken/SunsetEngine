//
// Created by sunvy on 28/06/2026.
//

#pragma once

#include "Rect.hpp"
#include "Widget.h"

namespace SRmGUI
{
    class Context
    {
    public:
        void SetRoot(const WidgetPtr& root);

        void Update(float dt);
        void HandleEvent();
        void SetLayout(const Rect& viewportRect);
        void Paint();
    private:
        WidgetPtr m_Root = nullptr;

        Rect m_ViewportRect;
    };
} // SRmGUi