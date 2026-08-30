//
// Created by sunvy on 29/08/2026.
//

#include "Renderer.h"

#include "DrawCmd.h"
#include "RenderCommand.h"
#include "RenderScene.h"

namespace Sunset
{
    void Renderer::RenderScene(const Sunset::RenderScene &scene)
    {
        RenderCommand::BeginFrame();

        RenderCommand::UseCamera(scene.m_Camera);

        // for (const auto& object : scene.GetObjects())
        // {
        //     RenderCommand::Submit(object.mesh, object.transform);
        // }
        RenderCommand::EndFrame();
    }
} // Sunset