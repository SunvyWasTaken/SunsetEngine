//
// Created by sunvy on 29/08/2026.
//

#pragma once
#include "Render/Resources/Camera.h"

namespace Sunset
{
    struct DrawCmd;

    class RenderScene
    {
        friend class BuildRenderScene;
    public:
        void BeginScene(const Camera& camera);

        const std::vector<DrawCmd>& GetObjects();

    private:
        std::vector<DrawCmd> m_ObjectToRender;
        Camera m_Camera;
    };
} // Sunset