//
// Created by sunvy on 07/07/2026.
//

#include "NativeScriptComponent.h"

#include "GameFramework/World/ScriptEntity.h"

namespace Sunset
{
    NativeScriptComponent::NativeScriptComponent()
        : m_ScriptEntitys()
        , InstantiateScriptEntity()
        // , DestroyScriptEntity(nullptr)
    {
    }

    void NativeScriptComponent::Start(World* world, const Entity& entity)
    {
        for (auto& i : InstantiateScriptEntity)
        {
            m_ScriptEntitys.emplace_back(i());
            m_ScriptEntitys.back()->m_Entity = entity;
            m_ScriptEntitys.back()->OnBeginPlay();
        }
    }
} // Sunset