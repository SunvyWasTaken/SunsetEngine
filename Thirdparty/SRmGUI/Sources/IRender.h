//
// Created by sunvy on 28/06/2026.
//

#pragma once

namespace SRmGUI
{
    enum class RenderAPI
    {
        OpenGL,
        Vulkan
    };

    struct RenderCreateInfo
    {
        void* WindowHandle = nullptr;
        int Width = 0;
        int Height = 0;
        bool vSync = false;
    };

    class IRender
    {
    };
} // SRmGUI