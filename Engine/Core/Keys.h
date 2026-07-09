//
// Created by sunvy on 09/07/2026.
//

#pragma once

namespace Sunset
{
    enum class Key : std::uint16_t
    {
        Unknown = 0,

        Space,
        Apostrophe,
        Comma,
        Minus,
        Period,
        Slash,

        Num0,
        Num1,
        Num2,
        Num3,
        Num4,
        Num5,
        Num6,
        Num7,
        Num8,
        Num9,

        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,

        Escape,
        Enter,
        Tab,
        Backspace,
        Insert,
        Delete,

        Right,
        Left,
        Down,
        Up,

        PageUp,
        PageDown,
        Home,
        End,

        CapsLock,
        ScrollLock,
        NumLock,
        PrintScreen,
        Pause,

        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,

        LeftShift,
        LeftControl,
        LeftAlt,
        LeftSuper,
        RightShift,
        RightControl,
        RightAlt,
        RightSuper,

        Menu,

        Count
    };
    enum class MouseKey : std::uint16_t
    {
        Unknown = 0,

        Left,
        Right,
        Middle,

        Button_4,
        Button_5,
        Button_6,
        Button_7,
        Button_8,

        Count
    };

    enum class GamepadButton : std::uint16_t
    {
        Unknown = 0,

        South,
        East,
        West,
        North,

        LeftBumper,
        RightBumper,
        Back,
        Start,
        Guide,

        LeftThumb,
        RightThumb,

        DPadUp,
        DPadRight,
        DPadDown,
        DPadLeft,

        Count
    };

    enum class GamepadAxis : std::uint16_t
    {
        Unknown = 0,

        LeftX,
        LeftY,
        RightX,
        RightY,
        LeftTrigger,
        RightTrigger,

        Count
    };
}
