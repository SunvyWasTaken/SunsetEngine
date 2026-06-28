//
// Created by sunvy on 15/12/2025.
//

#pragma once

#include "Input.h"
#include "../../Thirdparty/SRmGUI/Sources/Context.h"

namespace Sunset
{
    class Layer
    {
    public:
        Layer();
        virtual ~Layer() = default;

        virtual void OnUpdate(float dt);
        virtual void OnDraw();

        virtual bool OnEvent(Event::Type& event);
        void AddToViewport(const SRmGUI::WidgetPtr& widget);
        SRmGUI::Context& GetUIContext();
    private:
        SRmGUI::Context m_UIContext;
    };
}
