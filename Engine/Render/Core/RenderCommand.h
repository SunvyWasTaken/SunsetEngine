//
// Created by sunvy on 25/08/2026.
//

#pragma once

namespace Sunset
{
    class Drawable;
    struct RenderState;
    struct ShaderHandle;

    struct RenderCommand
    {
        static void SetRenderAPI(std::unique_ptr<struct RenderAPI> renderAPI);
        static bool HasRenderAPI();

        static void Shutdown();

        static void BeginFrame();
        static void EndFrame();

        static void Submit(const Drawable& drawable, const glm::mat4& transform);
    };
} // Sunset