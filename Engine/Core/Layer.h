//
// Created by sunvy on 15/12/2025.
//

#pragma once

#include "Core/Event.h"
#include "SRmGUI.h"

namespace Sunset
{
    class GameInstance;

    class Layer
    {
        friend class Application;
    public:
        Layer();
        virtual ~Layer();

        virtual void Init() {};

        virtual void OnUpdate(float dt);

        virtual void OnDraw();

        virtual bool OnEvent(Event::Type& event);

        template <typename T>
        [[nodiscard]]
        T* GetGameInstance()
        {
            if (!m_GameInstance)
                return nullptr;

            return static_cast<T*>(m_GameInstance);
        }

        void AddToViewport(const SRmGUI::WidgetPtr& widget);
        SRmGUI::Context& GetUIContext();

    private:
        void SetAppContext(GameInstance* gameInstance);
    private:
        GameInstance* m_GameInstance = nullptr;
        SRmGUI::Context& m_UIContext;
    };
}
