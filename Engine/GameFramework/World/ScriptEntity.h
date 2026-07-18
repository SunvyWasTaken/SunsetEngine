//
// Created by sunvy on 07/07/2026.
//

#pragma once
#include "Entity.h"

namespace Sunset
{
    class ScriptEntity
    {
    public:
        virtual ~ScriptEntity();

        virtual ReflectionType Properties();

        virtual void OnBeginPlay() {};

        virtual void OnUpdate(float dt) {};

        virtual void OnEndPlay() {};

        template <typename T>
        [[nodiscard]]
        T* GetComponent()
        {
            return m_Entity.GetComponent<T>();
        }

    private:
        Entity m_Entity;
        friend class NativeScriptComponent;
    };
} // Sunset