//
// Created by sunvy on 05/07/2026.
//

#include "Panel.h"

namespace SRmGUI
{
    struct WidgetSwitch : public SRmGUI::Panel
    {
    public:
        void Paint(FormeDatas &out) override;
        void AddChild(const std::shared_ptr<Widget> &child) override;
        void SetActive(std::size_t index);
    private:
        std::size_t m_Index = 0;
    };
}
