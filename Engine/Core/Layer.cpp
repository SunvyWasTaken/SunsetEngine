//
// Created by sunvy on 15/12/2025.
//

#include "Layer.h"

#include "Core/Application.h"
#include "Core/ApplicationSetting.h"
#include "Sources/SRmGUI.h"

namespace Sunset
{
    Layer::Layer()
        : m_UIContext(SRmGUI::GetContext())
    {
        const auto& setting = Application::GetSetting();
        m_UIContext.SetLayout({{0, 0}, setting.WindowSize});
    }

    Layer::~Layer()
    {
        SRmGUI::Shutdown();
    }

    void Layer::OnUpdate(float dt)
    {
        m_UIContext.Update(dt);
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
        }
        return false;
    }

    void Layer::AddToViewport(const SRmGUI::WidgetPtr &widget)
    {
        m_UIContext.SetRoot(widget);
    }

    SRmGUI::Context & Layer::GetUIContext()
    {
        return m_UIContext;
    }
}
