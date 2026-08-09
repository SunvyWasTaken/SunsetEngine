//
// Created by sunvy on 09/08/2026.
//

#include "SpriteRenderSystem.h"

#include "GameFramework/Components/SpriteRenderComponent.h"
#include "GameFramework/Components/TransformComponent.h"
#include "GameFramework/World/World.h"
#include "GameFramework/World/Entity.h"
#include "Render/Core/RenderCommand.h"

namespace Sunset
{
    void SpriteRenderSystem::Update(const float dt)
    {
        IWorldSystem::Update(dt);
        m_World->Each<TransformComponent, SpriteRenderComponent>([&](const Entity& entity, const TransformComponent& trans, const SpriteRenderComponent& sprc)
        {
            RenderCommand::Submit(sprc.m_drawable, trans.GetWorldMatrix());
        });
    }
} // Sunset