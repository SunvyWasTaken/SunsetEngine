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

        void ComputeDesiredSize() override;

        void Arrange(const Rectangle& parentRect) override;

        void Paint(UIRenderList &renderList) override;

        void AddChild(const std::shared_ptr<Widget>& widget);

        void ClearChildren();

        [[nodiscard]]
        const std::vector<std::shared_ptr<Widget>>& GetChildren() const;

        Widget* HitTest(const glm::ivec2& mouse) override;

    private:
        std::vector<std::shared_ptr<Widget>> m_Children;
    };
} // Sunset