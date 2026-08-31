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

    void Renderer::SetViewport(const glm::ivec2& viewportSize)
    {
        m_API->SetViewport(viewportSize);
    }

    void Renderer::RenderScene(const Sunset::RenderScene &scene) const
    {
        m_API->BeginScene(scene.m_Camera);

        m_API->Clear({1.f, 0.f, 0.f, 0.f});

        for (const auto& object : scene.GetObjects())
        {
            m_API->Submit(*object.m_Drawable, object.transform);
        }
        m_API->EndScene();
    }
} // Sunset