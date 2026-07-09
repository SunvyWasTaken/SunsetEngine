//
// Created by sunvy on 15/12/2025.
//

#include "Input.h"

namespace
{

}

namespace Sunset
{
    bool ActionValue::AsBool() const
    {
        return value.x != 0.0f || value.y != 0.0f;
    }

    float ActionValue::AsFloat() const
    {
        return value.x;
    }

    glm::vec2 ActionValue::AsVector2() const
    {
        return value;
    }

    void InputSystem::Begin()
    {
        Previous = Current;
        PreviousMouse = CurrentMouse;
        PreviousGamepads = CurrentGamepads;
        CurrentMouse.Delta = glm::vec2(0.0f);
        CurrentMouse.Scroll = glm::vec2(0.0f);
    }

    void InputSystem::ProcessEvent(const Event::Type& event)
    {
        if (std::holds_alternative<Event::Keyboard>(event))
        {
            const auto& keyboard = std::get<Event::Keyboard>(event);
            if (keyboard.action != Event::ButtonAction::Repeat)
                SetKey(keyboard.key, keyboard.action == Event::ButtonAction::Press);
        }
        else if (std::holds_alternative<Event::MouseButton>(event))
        {
            const auto& mouse = std::get<Event::MouseButton>(event);
            SetMousePosition(mouse.position);
            if (mouse.action != Event::ButtonAction::Repeat)
                SetMouseButton(mouse.key, mouse.action == Event::ButtonAction::Press);
        }
        else if (std::holds_alternative<Event::MouseMove>(event))
        {
            const auto& mouse = std::get<Event::MouseMove>(event);
            SetMousePosition(mouse.position);
        }
        else if (std::holds_alternative<Event::MouseScroll>(event))
        {
            const auto& mouse = std::get<Event::MouseScroll>(event);
            SetMousePosition(mouse.position);
            AddMouseScroll(mouse.offset);
        }
        else if (std::holds_alternative<Event::GamepadConnection>(event))
        {
            const auto& gamepad = std::get<Event::GamepadConnection>(event);
            SetGamepadConnected(gamepad.gamepad, gamepad.connected);
        }
        else if (std::holds_alternative<Event::GamepadButton>(event))
        {
            const auto& gamepad = std::get<Event::GamepadButton>(event);
            if (gamepad.action != Event::ButtonAction::Repeat)
                SetGamepadButton(gamepad.gamepad, gamepad.button, gamepad.action == Event::ButtonAction::Press);
        }
        else if (std::holds_alternative<Event::GamepadAxis>(event))
        {
            const auto& gamepad = std::get<Event::GamepadAxis>(event);
            SetGamepadAxis(gamepad.gamepad, gamepad.axis, gamepad.value);
        }
    }

    void InputSystem::SetKey(const Key key, const bool down)
    {
        Current.SetKey(key, down);
    }

    void InputSystem::SetMouseButton(const MouseKey key, const bool down)
    {
        CurrentMouse.Buttons.SetKey(key, down);
    }

    void InputSystem::SetMousePosition(const glm::vec2 position)
    {
        CurrentMouse.Delta += position - CurrentMouse.Position;
        CurrentMouse.Position = position;
    }

    void InputSystem::AddMouseScroll(const glm::vec2 offset)
    {
        CurrentMouse.Scroll += offset;
    }

    void InputSystem::SetGamepadConnected(const std::uint8_t gamepad, const bool connected)
    {
        if (gamepad >= CurrentGamepads.size())
            return;
        CurrentGamepads[gamepad].Connected = connected;
        if (!connected)
            CurrentGamepads[gamepad] = {};
    }

    void InputSystem::SetGamepadButton(const std::uint8_t gamepad, const GamepadButton button, const bool down)
    {
        if (gamepad >= CurrentGamepads.size())
            return;
        CurrentGamepads[gamepad].Buttons.SetKey(button, down);
    }

    void InputSystem::SetGamepadAxis(const std::uint8_t gamepad, const GamepadAxis axis, const float value)
    {
        if (gamepad >= CurrentGamepads.size())
            return;
        CurrentGamepads[gamepad].Axes[static_cast<std::uint16_t>(axis)] = value;
    }

    bool InputSystem::IsKeyDown(const Key key) const
    {
        return Current.IsDown(key);
    }

    bool InputSystem::IsPressed(const Key key) const
    {
        return Current.IsDown(key) && !Previous.IsDown(key);;
    }

    bool InputSystem::IsReleased(const Key key) const
    {
        return !Current.IsDown(key) && Previous.IsDown(key);
    }

    bool InputSystem::IsMouseButtonDown(const MouseKey key) const
    {
        return CurrentMouse.Buttons.IsDown(key);
    }

    bool InputSystem::IsMouseButtonPressed(const MouseKey key) const
    {
        return CurrentMouse.Buttons.IsDown(key) && !PreviousMouse.Buttons.IsDown(key);
    }

    bool InputSystem::IsMouseButtonReleased(const MouseKey key) const
    {
        return !CurrentMouse.Buttons.IsDown(key) && PreviousMouse.Buttons.IsDown(key);
    }

    glm::vec2 InputSystem::GetMousePosition() const
    {
        return CurrentMouse.Position;
    }

    glm::vec2 InputSystem::GetMouseDelta() const
    {
        return CurrentMouse.Delta;
    }

    glm::vec2 InputSystem::GetMouseScroll() const
    {
        return CurrentMouse.Scroll;
    }

    bool InputSystem::IsGamepadConnected(const std::uint8_t gamepad) const
    {
        return gamepad < CurrentGamepads.size() && CurrentGamepads[gamepad].Connected;
    }

    bool InputSystem::IsGamepadButtonDown(const std::uint8_t gamepad, const GamepadButton button) const
    {
        return gamepad < CurrentGamepads.size() && CurrentGamepads[gamepad].Buttons.IsDown(button);
    }

    bool InputSystem::IsGamepadButtonPressed(const std::uint8_t gamepad, const GamepadButton button) const
    {
        return gamepad < CurrentGamepads.size()
            && CurrentGamepads[gamepad].Buttons.IsDown(button)
            && !PreviousGamepads[gamepad].Buttons.IsDown(button);
    }

    bool InputSystem::IsGamepadButtonReleased(const std::uint8_t gamepad, const GamepadButton button) const
    {
        return gamepad < CurrentGamepads.size()
            && !CurrentGamepads[gamepad].Buttons.IsDown(button)
            && PreviousGamepads[gamepad].Buttons.IsDown(button);
    }

    float InputSystem::GetGamepadAxis(const std::uint8_t gamepad, const GamepadAxis axis) const
    {
        if (gamepad >= CurrentGamepads.size())
            return 0.0f;
        return CurrentGamepads[gamepad].Axes[static_cast<std::uint16_t>(axis)];
    }

    void InputMapping::Bind(Key key, const InputAction action)
    {
        m_Bindings[action].emplace_back(key);
    }

    void InputMapping::Bind(const MouseKey key, const InputAction action)
    {
        m_MouseBindings[action].emplace_back(key);
    }

    void InputMapping::Bind(const GamepadButton button, const InputAction action, const std::uint8_t gamepad)
    {
        m_GamepadButtonBindings[action].push_back({gamepad, button});
    }

    void InputMapping::BindAxis(const GamepadAxis axis, const InputAction action, const std::uint8_t gamepad, const float scale)
    {
        m_GamepadAxisBindings[action].push_back({gamepad, axis, scale});
    }

    bool InputMapping::IsActionDown(const InputAction action, const InputSystem &input) const
    {
        if (const auto it = m_Bindings.find(action); it != m_Bindings.end())
            for (const auto& key : it->second)
                if (input.IsKeyDown(key))
                    return true;

        if (const auto mouseIt = m_MouseBindings.find(action); mouseIt != m_MouseBindings.end())
            for (const auto& key : mouseIt->second)
                if (input.IsMouseButtonDown(key))
                    return true;

        if (const auto gamepadIt = m_GamepadButtonBindings.find(action); gamepadIt != m_GamepadButtonBindings.end())
            for (const auto& binding : gamepadIt->second)
                if (input.IsGamepadButtonDown(binding.gamepad, binding.button))
                    return true;

        return false;
    }

    bool InputMapping::IsActionPressed(const InputAction action, const InputSystem& input) const
    {
        if (const auto it = m_Bindings.find(action); it != m_Bindings.end())
            for (const auto& key : it->second)
                if (input.IsPressed(key))
                    return true;

        if (const auto it = m_MouseBindings.find(action); it != m_MouseBindings.end())
            for (const auto& key : it->second)
                if (input.IsMouseButtonPressed(key))
                    return true;

        if (const auto it = m_GamepadButtonBindings.find(action); it != m_GamepadButtonBindings.end())
            for (const auto& binding : it->second)
                if (input.IsGamepadButtonPressed(binding.gamepad, binding.button))
                    return true;

        return false;
    }

    bool InputMapping::IsActionReleased(const InputAction action, const InputSystem& input) const
    {
        if (const auto it = m_Bindings.find(action); it != m_Bindings.end())
            for (const auto& key : it->second)
                if (input.IsReleased(key))
                    return true;

        if (const auto it = m_MouseBindings.find(action); it != m_MouseBindings.end())
            for (const auto& key : it->second)
                if (input.IsMouseButtonReleased(key))
                    return true;

        if (const auto it = m_GamepadButtonBindings.find(action); it != m_GamepadButtonBindings.end())
            for (const auto& binding : it->second)
                if (input.IsGamepadButtonReleased(binding.gamepad, binding.button))
                    return true;

        return false;
    }

    ActionValue InputMapping::GetActionValue(const InputAction action, const InputSystem& input) const
    {
        ActionValue actionValue;
        if (IsActionDown(action, input))
            actionValue.value.x = 1.0f;

        if (const auto it = m_GamepadAxisBindings.find(action); it != m_GamepadAxisBindings.end())
            for (const auto& binding : it->second)
                actionValue.value.x += input.GetGamepadAxis(binding.gamepad, binding.axis) * binding.scale;

        return actionValue;
    }
}
