//
// Created by sunvy on 09/07/2026.
//

#pragma once

#include "Component.h"
#include "Core/Event.h"
#include "Core/Input.h"

namespace Sunset
{
    enum class InputBindingType : std::uint8_t
    {
        Keyboard,
        Mouse,
        GamepadButton,
        GamepadAxis
    };

    struct InputBindingInfo
    {
        InputBindingType Type = InputBindingType::Keyboard;
        InputAction Action = 0;
        Key KeyboardKey = Key::Unknown;
        MouseKey MouseButton = MouseKey::Unknown;
        GamepadButton PadButton = GamepadButton::Unknown;
        GamepadAxis PadAxis = GamepadAxis::Unknown;
        std::uint8_t Gamepad = 0;
        float Scale = 1.0f;
    };

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

        float MoveX() const;
        float MoveY() const;

        const InputSystem& GetInputSystem() const;
        std::vector<InputBindingInfo>& GetBindings();
        const std::vector<InputBindingInfo>& GetBindings() const;
        void AddBinding(const InputBindingInfo& binding);
        void RemoveBinding(std::size_t index);
        void RebuildMapping();

    protected:
        InputSystem inputSystem;
        InputMapping mapping;
        std::vector<InputBindingInfo> bindings;
    };
} // Sunset
