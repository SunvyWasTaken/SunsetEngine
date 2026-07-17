//
// Created by sunvy on 15/12/2025.
//

#pragma once

namespace Sunset
{
    class Camera;
    class Drawable;
    class FrameBuffer;

    class RenderAPI
    {
    public:
        virtual ~RenderAPI() = default;

        virtual void Init() = 0;
        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;

        virtual void BeginTarget(FrameBuffer& target, const glm::vec4& clearColor) = 0;
        virtual void EndTarget() = 0;
        virtual void Flush() = 0;

        virtual void Submit(const Drawable& drawable, const glm::mat4& model) = 0;
        virtual void UseCamera(const Camera& camera) = 0;
        virtual void ShowCursor(bool show) = 0;

        virtual void SetViewport(const glm::ivec2& size) = 0;
        virtual void Clear(const glm::vec4& color) = 0;
    };
}