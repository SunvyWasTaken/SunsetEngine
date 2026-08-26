//
// Created by sunvy on 09/08/2026.
//

#include "CameraSystem.h"

#include "GameFramework/Components/CameraComponent.h"
#include "GameFramework/World/World.h"
#include "GameFramework/World/Entity.h"
#include "Render/Core/RenderCommand.h"

namespace Sunset
{
    void CameraSystem::Update(float dt)
    {
        IWorldSystem::Update(dt);
        m_World->Each<Sunset::CameraComponent>([&](const Entity& entity, CameraComponent& component)
        {

        });
    }
} // Sunset