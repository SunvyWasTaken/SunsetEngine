//
// Created by sunvy on 29/08/2026.
//

#pragma once

namespace Sunset
{
    class RenderScene;
    class World;

    struct BuildRenderScene
    {
        void operator()(World& world, RenderScene& scene);
    };
} // Sunset