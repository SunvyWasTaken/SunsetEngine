//
// Created by sunvy on 29/06/2026.
//

#pragma once

#include "Widget.h"

namespace SRmGUI
{
    class Panel : public Widget
    {
    public:
        void Update(float dt) override;
        void Paint(FormeDatas &out) override;

        virtual void AddChild(const std::shared_ptr<Widget>& child);
    protected:
        std::vector<WidgetPtr> childs;
    };
}
