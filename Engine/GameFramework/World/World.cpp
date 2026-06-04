//
// Created by sunvy on 03/06/2026.
//

#include "World.h"

#include "Component.h"
#include "Entity.h"
#include "Render/RenderCommande.h"

namespace Sunset
{
    World::World()
    {
    }

    World::~World()
    {
    }

    void World::Update(float deltatime)
    {
        SS_PROFILE_FUNCTION();
        auto group = m_Registry.view<TransformComponent, MeshComponent>();
        for (const auto& entity : group)
        {
            const auto& [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
            RenderCommande::Submit(mesh.m_mesh);
        }
    }

    Entity World::CreateEntity(const std::string &name)
    {
        Entity entity{this, m_Registry.create()};
        entity.AddComponent<TagComponent>(name);
        return entity;
    }
} // Sunset