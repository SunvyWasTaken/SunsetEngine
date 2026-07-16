//
// Created by sunvy on 16/07/2026.
//

#include "RenderMeshSystem.h"

#include "GameFramework/Components/TransformComponent.h"
#include "GameFramework/World/Entity.h"
#include "GameFramework/World/World.h"
#include "../../Render/Core/RenderCommand.h"

namespace Sunset
{
    void RenderMeshSystem::Update(float dt)
    {
        IWorldSystem::Update(dt);
        m_World->Each<TransformComponent, MeshComponent>([&](const Entity& entity, const TransformComponent& trans, const MeshComponent& mesh)
        {
            RenderCommand::Submit(mesh.m_mesh);
        });
    }
} // Sunset