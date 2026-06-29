//
// Created by sunvy on 15/12/2025.
//

#pragma once

#include "Input.h"
#include "SRmGUI.h"

namespace Sunset
{
    class Layer
    {
    public:
        Layer();
        virtual ~Layer();

        virtual void OnUpdate(float dt);
        virtual void OnDraw();

        virtual bool OnEvent(Event::Type& event);
        void AddToViewport(const SRmGUI::WidgetPtr& widget);
        SRmGUI::Context& GetUIContext();
    private:
        SRmGUI::Context& m_UIContext;
    };
}
