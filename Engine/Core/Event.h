//
// Created by sunvy on 09/07/2026.
//

#pragma once

#include "Keys.h"

namespace Sunset::Event
{
    struct Keyboard
    {
        Key key;
        bool Press = false;
    };

    struct Mouse
    {
        MouseKey key;
        glm::vec2 position;
    };

    struct Window
    {
        glm::vec2 size;
    };

    using Type = std::variant<Keyboard, Mouse, Window>;
} // Sunset::Event