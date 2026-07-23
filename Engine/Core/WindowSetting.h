//
// Created by sunvy on 15/12/2025.
//

#pragma once

namespace Sunset
{
    enum class WindowGraphicsAPI
    {
        None,
        OpenGL,
        Vulkan
    };

    struct WindowSetting
    {
        std::string_view WindowTitle{"SunsetEngine"};
        glm::ivec2 WindowSize{1280, 720};
        WindowGraphicsAPI GraphicsAPI = WindowGraphicsAPI::OpenGL;
        bool vSync{true};
        float HeadlessTickRate{60.0f};
        bool Headless{false};
    };
}
