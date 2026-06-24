//
// Created by sunvy on 15/12/2025.
//

#include "Layer.h"

namespace Sunset
{
    bool Layer::OnEvent(Event::Type& event)
    {
        return false;
    }

    UIContext & Layer::GetUIContext()
    {
        return m_UIContext;
    }
}
