//
// Created by sunvy on 15/12/2025.
//

#ifndef SUNSETCRAFT_LAYER_H
#define SUNSETCRAFT_LAYER_H
#include "Input.h"
#include "Widget/UIContext.h"

namespace Sunset
{
    class Layer
    {
    public:
        Layer() = default;
        virtual ~Layer() = default;

        virtual void OnUpdate(float dt) = 0;
        virtual void OnDraw() = 0;

        virtual bool OnEvent(Event::Type& event);
        UIContext& GetUIContext();
    private:
        UIContext m_UIContext;
    };
}

#endif //SUNSETCRAFT_LAYER_H