//
// Created by sunvy on 05/07/2026.
//

#include "WidgetSwitch.h"

#include "Type.h"

namespace SRmGUI
{
    void WidgetSwitch::Paint(FormeDatas &out)
    {
        if (m_Index >= childs.size() || m_Index < 0)
        {
            childs[0]->Paint(out);
            return;
        }
        childs[m_Index]->Paint(out);
    }

    void WidgetSwitch::AddChild(const std::shared_ptr<Widget> &child)
    {
        Panel::AddChild(child);
        for (const auto &c : childs)
            c->SetVisibility(false);

        childs[m_Index]->SetVisibility(true);
    }

    void WidgetSwitch::SetActive(const std::size_t index)
    {
        m_Index = index;
        for (const auto &c : childs)
            c->SetVisibility(false);

        childs[m_Index]->SetVisibility(true);
    }
}
