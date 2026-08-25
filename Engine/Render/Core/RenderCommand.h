//
// Created by sunvy on 25/08/2026.
//

#pragma once

namespace Sunset
{
    struct RenderCommand
    {
        static void SetRenderAPI(std::unique_ptr<struct RenderAPI> renderAPI);
        static bool HasRenderAPI();

        static void Shutdown();

        static void BeginFrame();
        static void EndFrame();
    };
} // Sunset