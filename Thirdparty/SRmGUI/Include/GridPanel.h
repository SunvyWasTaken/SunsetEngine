//
// Created by sunvy on 30/06/2026.
//

#pragma once

#include "Panel.h"

namespace SRmGUI
{
    class GridPanel : public Panel
    {
    public:
        void Arrange(const Rect &viewportRect) override;
        void SetColumn(int32_t nbr);
        void SetRow(int32_t nbr);
    private:
        glm::ivec2 m_NbrGrid{1, 1};
    };

    template<>
class WidgetBuilder<SRmGUI::GridPanel>
: public WidgetBuilderBase<SRmGUI::GridPanel, WidgetBuilder<SRmGUI::GridPanel>>
    {
    public:
        WidgetBuilder& Column(const int32_t nbr)
        {
            m_Widget->SetColumn(nbr);
            return *this;
        }

        WidgetBuilder& Row(const int32_t nbr)
        {
            m_Widget->SetRow(nbr);
            return *this;
        }
    };
}
