//
// Created by sunvy on 09/07/2026.
//

#include "InputComponent.h"

namespace Sunset
{
    void InputComponent::BeginFrame()
    {
        inputSystem.Begin();
    }

    void InputComponent::OnEvent(const Event::Type &event)
    {
        inputSystem.ProcessEvent(event);
    }

    void InputComponent::BindAction(const Key key, const InputAction action)
    {
        mapping.Bind(key, action);
    }

    void InputComponent::BindAction(const MouseKey key, const InputAction action)
    {
        mapping.Bind(key, action);
    }

    void InputComponent::BindAction(const GamepadButton button, const InputAction action, const std::uint8_t gamepad)
    {
        mapping.Bind(button, action, gamepad);
    }

    void InputComponent::BindAxis(const GamepadAxis axis, const InputAction action, const std::uint8_t gamepad, const float scale)
    {
        mapping.BindAxis(axis, action, gamepad, scale);
    }

    bool InputComponent::IsActionDown(const InputAction action) const
    {
        return mapping.IsActionDown(action, inputSystem);
    }

    bool InputComponent::IsActionPressed(const InputAction action) const
    {
        return mapping.IsActionPressed(action, inputSystem);
    }

    bool InputComponent::IsActionReleased(const InputAction action) const
    {
        return mapping.IsActionReleased(action, inputSystem);
    }

    ActionValue InputComponent::GetActionValue(const InputAction action) const
    {
        return mapping.GetActionValue(action, inputSystem);
    }

    const InputSystem& InputComponent::GetInputSystem() const
    {
        return inputSystem;
    }
} // Sunset
