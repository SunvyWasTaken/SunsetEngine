//
// Created by sunvy on 16/07/2026.
//

#pragma once

#include "IWorldSystem.h"

namespace Sunset
{
    class NativeScriptingSystem : public IWorldSystem
    {
    public:
        using IWorldSystem::IWorldSystem;
        void Update(float dt) override;
    };
} // Sunset