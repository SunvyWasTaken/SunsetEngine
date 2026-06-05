//
// Created by sunvy on 05/01/2026.
//

#pragma once

namespace Sunset
{
    class Mesh;
    class Camera;
    class Drawable;

    struct RenderCommande
    {
        static void BeginFrame();
        static void EndFrame();

        static void Submit(const Drawable& drawable);
        static void Submit(const Drawable& mesh, const glm::mat4& transform);
        static void UseCamera(const Camera& camera);
    };
}
