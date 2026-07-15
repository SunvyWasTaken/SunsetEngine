//
// Created by sunvy on 07/07/2026.
//

#include "NativeScriptComponent.h"

#include "GameFramework/World/ScriptEntity.h"

namespace Sunset
{
    NativeScriptComponent::NativeScriptComponent()
        : m_ScriptEntity(nullptr)
        , InstantiateScriptEntity(nullptr)
        , DestroyScriptEntity(nullptr)
    {
    }

    ReflectionType NativeScriptComponent::Properties()
    {
        if (!m_ScriptEntity)
            return Component::Properties();

        return m_ScriptEntity->Properties();
    }
} // Sunset