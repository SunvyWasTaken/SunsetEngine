//
// Created by sunvy on 16/07/2026.
//

#include "InputSystem.h"

#include "GameFramework/Components/InputComponent.h"
#include "GameFramework/World/Entity.h"
#include "GameFramework/World/World.h"

namespace Sunset
{
    void InputWorldSystem::Update(float dt)
    {
        IWorldSystem::Update(dt);
        m_World->Each<InputComponent>([&](const Entity& entity, InputComponent& inputComponent)
        {
            inputComponent.EndFrame();
        });
    }
} // Sunset