//
// Created by sunvy on 16/07/2026.
//

#include "NativeScriptingSystem.h"

#include "GameFramework/Components/NativeScriptComponent.h"
#include "GameFramework/World/ScriptEntity.h"
#include "GameFramework/World/World.h"

namespace Sunset
{
    void NativeScriptingSystem::Update(const float dt)
    {
        IWorldSystem::Update(dt);
        m_World->Each<NativeScriptComponent>([&](const Entity& entity, NativeScriptComponent& script)
        {
            for (const auto& it : script.m_ScriptEntitys)
              it->OnUpdate(dt);
        });
    }
} // Sunset