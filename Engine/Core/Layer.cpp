//
// Created by sunvy on 15/12/2025.
//

#include "Layer.h"

#include "Core/Application.h"
#include "Core/ApplicationSetting.h"

namespace Sunset
{
    void Layer::OnUpdate(float dt)
    {
        m_UIContext.Update(dt);
        m_UIContext.PrePass();
        const auto& setting = Application::GetSetting();
        m_UIContext.Arrange(static_cast<float>(setting.WindowSize.x), static_cast<float>(setting.WindowSize.y));
    }

    void Layer::OnDraw()
    {
        m_UIContext.Paint();
    }

    bool Layer::OnEvent(Event::Type& event)
    {
        if (std::holds_alternative<Event::MouseEvent>(event))
        {
            const auto& mouseEvent = std::get<Event::MouseEvent>(event);
            if (m_UIContext.ProcessMouseButton(mouseEvent.button, mouseEvent.action, mouseEvent.position))
                return true;
        }
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
