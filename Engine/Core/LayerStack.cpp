//
// Created by sunvy on 16/12/2025.
//

#include "LayerStack.h"

namespace Sunset
{
    void LayerStack::Clear()
    {
        if (!m_Layers.empty())
            m_Layers.clear();

        m_OverlayPosition = 0;
    }
}
