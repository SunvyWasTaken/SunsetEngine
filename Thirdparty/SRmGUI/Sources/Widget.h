//
// Created by sunvy on 28/06/2026.
//

#pragma once

#include "SRmGUI_fwd.h"

namespace SRmGUI
{
    class Widget : public std::enable_shared_from_this<Widget>
    {
    public:
        void SetParent(const WidgetPtr& parent);
        WidgetPtr GetSelf();

        virtual void Update(float dt);

        virtual void Paint(FormeDatas& out);
    protected:
        WidgetWeak m_Parent;
    };
} // SRmGUI