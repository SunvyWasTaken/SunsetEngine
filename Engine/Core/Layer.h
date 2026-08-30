//
// Created by sunvy on 15/12/2025.
//

#pragma once

#include "Core/Event.h"

namespace Sunset
{
    class Renderer;

    class Layer
    {
        friend class Application;
    public:
        Layer() = default;

        virtual ~Layer() = default;

        virtual void Init() {};

        virtual void OnUpdate(float dt);

        virtual void OnDraw(Renderer* renderer);

        virtual bool OnEvent(const Event::Type& event);
    };
}
