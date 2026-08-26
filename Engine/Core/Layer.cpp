//
// Created by sunvy on 15/12/2025.
//

#include "Layer.h"

#include "Core/Application.h"
#include "Core/WindowSetting.h"
#include "GameInstance.h"
#include "SRmGUI.h"
#include "GameFramework/Components/InputComponent.h"
#include "GameFramework/World/World.h"

namespace Sunset
{
    Layer::Layer()
        : m_UIContext(SRmGUI::CreateContext())
    {
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
        const auto& setting = Application::GetSetting();
        m_UIContext.Arrange({{0, 0}, setting.WindowSize});
        m_UIContext.Paint();
    }

    bool Layer::OnEvent(const Event::Type& event)
    {
        if (std::holds_alternative<Event::Mouse>(event))
        {
            const auto& mouseEvent = std::get<Event::Mouse>(event);
            m_UIContext.HandleEvent({mouseEvent.position, static_cast<SRmGUI::MouseEvent::Type>(1), static_cast<std::uint16_t>(mouseEvent.key)});
        }
        if (std::holds_alternative<Event::Keyboard>(event))
        {
            const auto& keyEvent = std::get<Event::Keyboard>(event);
            // m_UIContext.HandleEvent();
        }
        return false;
    }

    World* Layer::GetWorld() const
    {
        return m_GameInstance->m_ActiveWorld.get();
    }

    void Layer::AddToViewport(const SRmGUI::WidgetPtr &widget)
    {
        m_UIContext.SetRoot(widget);
    }

    SRmGUI::Context & Layer::GetUIContext()
    {
        return m_UIContext;
    }

    void Layer::SetAppContext(GameInstance *gameInstance)
    {
        m_GameInstance = gameInstance;
    }
}
