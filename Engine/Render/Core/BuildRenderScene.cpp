//
// Created by sunvy on 29/08/2026.
//

#include "BuildRenderScene.h"

#include "DrawCmd.h"
#include "RenderScene.h"
#include "GameFramework/Components/SpriteRenderComponent.h"
#include "GameFramework/Components/TransformComponent.h"
#include "GameFramework/World/Entity.h"
#include "GameFramework/World/World.h"
#include "Render/Resources/Material.h"
#include "Render/Resources/Mesh.h"

namespace Sunset
{
    void BuildRenderScene::operator()(World &world, RenderScene &scene)
    {
        scene.m_ObjectToRender.clear();

        world.Each<TransformComponent, SpriteRenderComponent>([&](const Entity&, const TransformComponent& transform, const SpriteRenderComponent& Sprc)
        {
            scene.m_ObjectToRender.emplace_back(Sprc.m_drawable.m_Mesh->Get(), Sprc.m_drawable.m_Material->Get(), transform.GetWorldMatrix());
        });

        // world.Each<TransformComponent, MeshComponent>([&](const Entity&, const TransformComponent& transform, const MeshComponent& mesh)
        // {
        //     scene.m_ObjectToRender.emplace_back(mesh.m_Mesh.Get(), mesh.m_Material.Get(), transform.GetWorldMatrix());
        // });
    }
} // Sunset