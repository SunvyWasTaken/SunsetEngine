//
// Created by sunvy on 09/08/2026.
//

#pragma once

#include "IWorldSystem.h"

namespace Sunset
{
    class SpriteRenderSystem : public IWorldSystem
    {
    public:
        using IWorldSystem::IWorldSystem;
        void Update(float dt) override;
    };
} // Sunset