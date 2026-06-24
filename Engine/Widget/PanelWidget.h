//
// Created by sunvy on 24/06/2026.
//

#pragma once

#include "Widget.h"

namespace Sunset
{
    class PanelWidget : public Widget
    {
    public:
        void Update(float dt) override;

        void Paint(UIRenderList &renderList) override;

        void AddChild(const std::shared_ptr<Widget>& widget);

        Widget* HitTest(const glm::ivec2& mouse) override;

    private:
        std::vector<std::shared_ptr<Widget>> m_Childrens;
    };
} // Sunset