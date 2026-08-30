//
// Created by sunvy on 29/08/2026.
//

#pragma once

#include "Render/Resources/Camera.h"
#include "DrawCmd.h"

namespace Sunset
{
    class RenderScene
    {
        friend class BuildRenderScene;
        friend class Renderer;
    public:
        ~RenderScene();

        void BeginScene(const Camera& camera);

        const std::vector<DrawCmd>& GetObjects() const;

    private:
        std::vector<DrawCmd> m_ObjectToRender;
        Camera m_Camera;
    };
} // Sunset