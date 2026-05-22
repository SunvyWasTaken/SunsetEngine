//
// Created by sunvy on 05/01/2026.
//

#ifndef SUNSETCRAFT_RENDERCOMMANDE_H
#define SUNSETCRAFT_RENDERCOMMANDE_H

namespace SunsetEngine
{
    class Camera;
    class Drawable;

    struct RenderCommande
    {
        static void BeginFrame();
        static void EndFrame();

        static void Submit(const Drawable& drawable);
        static void UseCamera(const Camera& camera);
    };
}

#endif //SUNSETCRAFT_RENDERCOMMANDE_H