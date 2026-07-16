//
// Created by sunvy on 15/12/2025.
//

#pragma once

namespace Sunset
{
    class RenderAPI
    {
    public:
        virtual ~RenderAPI() = default;

        virtual void Init() = 0;
        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;

        virtual void SetViewport(const glm::ivec2& size) = 0;
        virtual void Clear(const glm::vec4& color) = 0;
    };
}
