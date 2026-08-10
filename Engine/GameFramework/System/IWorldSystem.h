//
// Created by sunvy on 16/07/2026.
//

#pragma once

namespace Sunset
{
    class World;

    class IWorldSystem
    {
    public:
        explicit IWorldSystem(World* world = nullptr);
        virtual ~IWorldSystem() = default;
        virtual void Update(float dt) {};
    protected:
        World* m_World;
    };
} // Sunset