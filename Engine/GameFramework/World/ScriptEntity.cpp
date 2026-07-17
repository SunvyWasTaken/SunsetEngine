//
// Created by sunvy on 07/07/2026.
//

#include "ScriptEntity.h"

namespace Sunset
{
    ScriptEntity::~ScriptEntity()
    {
        LOG("Engine", info, "ScriptEntity::~ScriptEntity()")
    }

    ReflectionType ScriptEntity::Properties()
    {
        return {};
    }
} // Sunset