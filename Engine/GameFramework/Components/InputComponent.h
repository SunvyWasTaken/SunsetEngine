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
        void BindAction(Key key, InputAction action);
        void BindAction(MouseKey key, InputAction action);
        void BindAction(GamepadButton button, InputAction action, std::uint8_t gamepad = 0);
        void BindAxis(GamepadAxis axis, InputAction action, std::uint8_t gamepad = 0, float scale = 1.0f);
        bool IsActionDown(InputAction action) const;
        bool IsActionPressed(InputAction action) const;
        bool IsActionReleased(InputAction action) const;
        ActionValue GetActionValue(InputAction action) const;

        const InputSystem& GetInputSystem() const;

    private:
        InputSystem inputSystem;
        InputMapping mapping;
    };
} // Sunset
