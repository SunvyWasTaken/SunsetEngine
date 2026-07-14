//
// Created by sunvy on 09/07/2026.
//

#pragma once

#include "Keys.h"

namespace Sunset::Event
{
    enum class ButtonAction : std::uint8_t
    {
        Press,
        Release,
        Repeat
    };

    struct KeyModifiers
    {
        bool Shift = false;
        bool Control = false;
        bool Alt = false;
        bool Super = false;
    };

    struct Keyboard
    {
        Key key;
        ButtonAction action = ButtonAction::Release;
        KeyModifiers modifiers;
        std::int32_t scancode = 0;
    };

    struct TextInput
    {
        char32_t codepoint = 0;
    };

    struct MouseButton
    {
        MouseKey key;
        glm::f64vec2 position;
        ButtonAction action = ButtonAction::Release;
        KeyModifiers modifiers;
    };

    struct MouseMove
    {
        glm::f64vec2 position;
        glm::vec2 delta;
    };

    struct MouseScroll
    {
        glm::vec2 offset;
        glm::vec2 position;
    };

    struct GamepadButton
    {
        std::uint8_t gamepad = 0;
        Sunset::GamepadButton button = Sunset::GamepadButton::Unknown;
        ButtonAction action = ButtonAction::Release;
    };

    struct GamepadAxis
    {
        std::uint8_t gamepad = 0;
        Sunset::GamepadAxis axis = Sunset::GamepadAxis::Unknown;
        float value = 0.0f;
    };

    struct GamepadConnection
    {
        std::uint8_t gamepad = 0;
        bool connected = false;
    };

    struct Window
    {
        glm::vec2 size;
    };

    using Mouse = MouseButton;

    using Type = std::variant<
        Keyboard,
        TextInput,
        MouseButton,
        MouseMove,
        MouseScroll,
        GamepadButton,
        GamepadAxis,
        GamepadConnection,
        Window>;
} // Sunset::Event
