//
// Created by sunvy on 05/01/2026.
//

#pragma once

namespace Sunset
{
    class RenderAPI;
    class Mesh;
    class Camera;
    class Drawable;
    class FrameBuffer;

    struct RenderCommand
    {
        static void SetRenderAPI(std::unique_ptr<RenderAPI> api);

        static void BeginFrame();
        static void EndFrame();
        static void Shutdown();

        static void BeginTarget(FrameBuffer& target, const glm::vec4& clearColor = glm::vec4(0.0f));
        static void EndTarget();
        static void Flush();

        static void Submit(const Drawable& mesh, const glm::mat4& model);
        static void UseCamera(const Camera& camera);
        static void ShowCursor(bool show);
        static void SetViewport(const glm::ivec2& viewport);
    };
}
