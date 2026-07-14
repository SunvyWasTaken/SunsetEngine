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

    float InputComponent::MoveX() const
    {
        return inputSystem.GetMouseDelta().x;
    }

    float InputComponent::MoveY() const
    {
        return inputSystem.GetMouseDelta().y;
    }

    const InputSystem& InputComponent::GetInputSystem() const
    {
        return inputSystem;
    }

    std::vector<InputBindingInfo>& InputComponent::GetBindings()
    {
        return bindings;
    }

    const std::vector<InputBindingInfo>& InputComponent::GetBindings() const
    {
        return bindings;
    }

    void InputComponent::AddBinding(const InputBindingInfo& binding)
    {
        bindings.emplace_back(binding);
        RebuildMapping();
    }

    void InputComponent::RemoveBinding(const std::size_t index)
    {
        if (index >= bindings.size())
            return;

        bindings.erase(bindings.begin() + static_cast<std::ptrdiff_t>(index));
        RebuildMapping();
    }

    void InputComponent::RebuildMapping()
    {
        mapping = InputMapping{};
        for (const auto& binding : bindings)
        {
            switch (binding.Type)
            {
                case InputBindingType::Keyboard:
                    mapping.Bind(binding.KeyboardKey, binding.Action);
                    break;
                case InputBindingType::Mouse:
                    mapping.Bind(binding.MouseButton, binding.Action);
                    break;
                case InputBindingType::GamepadButton:
                    mapping.Bind(binding.PadButton, binding.Action, binding.Gamepad);
                    break;
                case InputBindingType::GamepadAxis:
                    mapping.BindAxis(binding.PadAxis, binding.Action, binding.Gamepad, binding.Scale);
                    break;
            }
        }
    }
} // Sunset
