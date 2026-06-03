//
// Created by sunvy on 03/06/2026.
//

#include "Entity.h"

namespace Sunset
{
    Entity::Entity(World *world, entt::entity id)
        : m_World(world)
        , m_Id(id)
    {
    }

    Entity::~Entity()
    {
    }
} // Sunset