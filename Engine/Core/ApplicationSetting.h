//
// Created by sunvy on 15/12/2025.
//

#pragma once

namespace Sunset
{
    struct ApplicationSetting
    {
        glm::ivec2 WindowSize{1280, 720};
        std::string WindowTitle{"SunsetEngine"};
        bool Headless = false;
        float HeadlessTickRate = 60.0f;
    };
}
