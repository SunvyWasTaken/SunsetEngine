//
// Created by sunvy on 15/12/2025.
//

#pragma once

namespace Sunset
{
    struct WindowSetting
    {
        std::string_view WindowTitle{"SunsetEngine"};
        glm::ivec2 WindowSize{1920, 1080};
        bool vSync{true};
        bool Headless{false};
        float HeadlessTickRate{60.0f};
    };
}
