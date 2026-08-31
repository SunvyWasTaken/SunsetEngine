//
// Created by sunvy on 29/08/2026.
//

#pragma once

namespace Sunset
{
    struct RenderAPI;
    class RenderScene;

    class Renderer
    {
    friend class Application;
    public:
        explicit Renderer(std::unique_ptr<RenderAPI> renderAPI);
        ~Renderer();
        void SetViewport(const glm::ivec2& viewportSize);
        void RenderScene(const RenderScene& scene) const;
    private:
        std::unique_ptr<RenderAPI> m_API;
    };
} // Sunset