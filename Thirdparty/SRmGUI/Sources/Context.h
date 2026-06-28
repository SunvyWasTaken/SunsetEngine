//
// Created by sunvy on 28/06/2026.
//

#pragma once

#include "Rect.hpp"
#include "SRmGUI_fwd.h"

namespace SRmGUI
{
    class Context
    {
    public:
        Context();
        ~Context();
        void SetRoot(const WidgetPtr& root);

        void Update(float dt);
        void HandleEvent();
        void SetLayout(const Rect& viewportRect);
        void Paint();
        FormeDatas& GetData();
    private:
        WidgetPtr m_Root = nullptr;
        Rect m_ViewportRect;
        FormeDatas m_FormeDatas;
    };
} // SRmGUi