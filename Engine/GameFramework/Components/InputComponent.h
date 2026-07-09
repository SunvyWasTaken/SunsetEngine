//
// Created by sunvy on 09/07/2026.
//

#pragma once

#include "Component.h"
#include "Core/Event.h"
#include "Core/Input.h"

namespace Sunset
{
    class InputComponent : public Component
    {
    public:
        void BeginFrame();
        void OnEvent(const Event::Type& event);
        bool IsActionDown(InputAction action) const;

    private:
        InputSystem inputSystem;
        InputMapping mapping;
    };
} // Sunset