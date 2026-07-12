//
// Created by sunvy on 12/07/2026.
//

#pragma once

namespace Sunset
{
    class World;

    class WorldManager
    {
    public:
        std::shared_ptr<World> CreateWorld();

        void SetActiveWorld(std::shared_ptr<World> world);

        std::shared_ptr<World> GetActiveWorld();

        World* GetActiveWorldRaw();

    private:
        std::shared_ptr<World> m_ActiveWorld;
    };
} // Sunset