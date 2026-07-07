//
// Created by sunvy on 07/07/2026.
//

#pragma once

namespace Sunset
{
    class ScriptEntity;

    class NativeScriptComponent
    {
    public:
        ScriptEntity* m_ScriptEntity;

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