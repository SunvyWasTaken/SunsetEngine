//
// Created by sunvy on 07/07/2026.
//

#pragma once
#include "Component.h"

namespace Sunset
{
    class ScriptEntity;

    class NativeScriptComponent : public Component
    {
    public:
        NativeScriptComponent();
        ScriptEntity* m_ScriptEntity;

        ReflectionType Properties() override;

        ScriptEntity* (*InstantiateScriptEntity)();
        void(*DestroyScriptEntity)(NativeScriptComponent*);

        template <typename T>
        requires(std::is_base_of_v<ScriptEntity, T>)
        void Bind()
        {
            InstantiateScriptEntity = [](){ return static_cast<ScriptEntity*>(new T()); };
            DestroyScriptEntity = [](NativeScriptComponent* self){ delete static_cast<T*>(self->m_ScriptEntity); };
        }
    };
} // Sunset