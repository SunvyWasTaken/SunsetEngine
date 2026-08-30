//
// Created by sunvy on 29/08/2026.
//

#include "Renderer.h"

#include "DrawCmd.h"
#include "RenderScene.h"
#include "RenderAPI.h"

namespace Sunset
{
    Renderer::Renderer(std::unique_ptr<RenderAPI> renderAPI)
        : m_API(std::move(renderAPI))
    {
        m_API->Init();
    }

    Renderer::~Renderer()
    {
    }

    void Renderer::RenderScene(const Sunset::RenderScene &scene)
    {
        m_API->BeginScene(scene.m_Camera);

        for (const auto& object : scene.GetObjects())
        {
            m_API->Submit(*object.m_Drawable, object.transform);
        }
        m_API->EndScene();
    }
} // Sunset