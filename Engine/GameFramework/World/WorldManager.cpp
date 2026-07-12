//
// Created by sunvy on 12/07/2026.
//

#include "WorldManager.h"

#include "World.h"

namespace Sunset
{
    std::shared_ptr<World> WorldManager::CreateWorld()
    {
        m_ActiveWorld = std::make_shared<World>();
        return m_ActiveWorld;
    }

    void WorldManager::SetActiveWorld(std::shared_ptr<World> world)
    {
        m_ActiveWorld = std::move(world);
    }

    std::shared_ptr<World> WorldManager::GetActiveWorld()
    {
        return m_ActiveWorld;
    }

    World * WorldManager::GetActiveWorldRaw()
    {
        return m_ActiveWorld.get();
    }
} // Sunset