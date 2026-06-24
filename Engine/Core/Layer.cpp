//
// Created by sunvy on 15/12/2025.
//

#include "Layer.h"

namespace Sunset
{
    void Layer::OnUpdate(float dt)
    {
    }

    void Layer::OnDraw()
    {
        m_UIContext.Paint();
    }

    bool Layer::OnEvent(Event::Type& event)
    {
        return false;
    }

    void Layer::AddToViewport(const std::shared_ptr<Widget> &widget)
    {
        m_UIContext.AddWidget(widget);
    }

    UIContext & Layer::GetUIContext()
    {
        return m_UIContext;
    }
}
