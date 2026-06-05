//
// Created by sunvy on 03/06/2026.
//

#include "World.h"

#include "GameFramework/Controller.h"
#include "Component.h"
#include "Entity.h"
#include "BaseObject/BaseCube.h"
#include "Core/Input.h"
#include "Render/RenderCommande.h"

namespace
{

}

namespace Sunset
{
    World::World()
        : m_Registry()
        , m_Controllers()
    {
        CreatePlayer(0);
    }

    World::~World()
    {
        LOG("Engine", trace, "World::~World()")
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
        for (auto& m : m_Controllers)
        {
            m.Update(deltatime);
            const auto& transform = m_Registry.get<TransformComponent>(m.GetEntity());
            DrawCube(transform, glm::vec4(1.0), true);
            PRINTSCREEN("Controller location {}", transform.GetLocation());
        }
    }

    Entity World::CreateEntity(const std::string &name)
    {
        Entity entity{this, m_Registry.create()};
        entity.AddComponent<TagComponent>(name);
        return entity;
    }

    void World::OnPeerConnected(PeerId peerId)
    {
        CreatePlayer(peerId, false);
    }

    void World::CreatePlayer(PeerId peer, bool local)
    {
        std::unique_ptr<IInputSource> inputSource = nullptr;
        if (local)
            inputSource = std::make_unique<LocalInputSource>();
        else
            inputSource = std::make_unique<NetworkInputSource>(peer);

        Controller playerController(peer, inputSource);

        Entity character = CreateEntity("Player");

        playerController.Possess(character);
        character.AddComponent<TransformComponent>();

        m_Controllers.emplace_back(std::move(playerController));
    }
} // Sunset