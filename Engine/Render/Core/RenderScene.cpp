//
// Created by sunvy on 29/08/2026.
//

#include "RenderScene.h"

namespace Sunset
{
    void RenderScene::BeginScene(const Camera &camera)
    {
        m_Camera = camera;
    }

    const std::vector<DrawCmd> & RenderScene::GetObjects()
    {
        return m_ObjectToRender;
    }
} // Sunset