//
// Created by sunvy on 15/12/2025.
//

#pragma once

namespace Sunset
{
    struct ApplicationSetting
    {
        std::string_view WindowTitle{"SunsetEngine"};
        glm::ivec2 WindowSize{1280, 720};
        float HeadlessTickRate = 60.0f;
        bool Headless = false;
    };
}
