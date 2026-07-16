//
// Created by sunvy on 03/06/2026.
//

#include "World.h"

#include "Entity.h"
#include "ScriptEntity.h"
#include "Core/Application.h"
#include "GameFramework/Components/CameraComponent.h"
#include "GameFramework/Components/InputComponent.h"
#include "GameFramework/Components/NativeScriptComponent.h"
#include "GameFramework/Components/TransformComponent.h"
#include "Network/NetworkService.h"
#include "Render/RenderCommande.h"

namespace Sunset
{
    World::World()
        : m_Registry()
    {
        if (NetworkService::IsInitialized())
        {
            NetworkService::Get().RegisterMessage<NetworkPlayerSessionMessage>(NetworkPlayerSessionMessage::ChannelId);
            NetworkService::Get().RegisterHandler<NetworkPlayerSessionMessage>([this](PeerId, const NetworkPlayerSessionMessage& msg)
            {
                OnPlayerSessionMessage(msg);
            });

            NetworkService::Get().RegisterPeerConnectedHandler([this](PeerId peerId)
            {
                OnPeerConnected(peerId);
            });

            NetworkService::Get().RegisterPeerDisconnectedHandler([this](PeerId peerId)
            {
                OnPeerDisconnected(peerId);
            });
        }
    }

    World::~World()
    {
        NetworkService::Shutdown();
        LOG("Engine", trace, "World::~World()")
    }

    bool World::OnEvent(const Event::Type &event)
    {
        m_Registry.view<InputComponent>().each([&](InputComponent& inputComponent)
        {
           inputComponent.OnEvent(event);
        });
        return false;
    }

    void World::Update(float dt)
    {
        SS_PROFILE_FUNCTION();
        m_Registry.view<NativeScriptComponent>().each([&](const entt::entity entity, NativeScriptComponent& script)
        {
            // Todo move the instantiate to the BeginPlayScene.
            if (script.m_ScriptEntitys.empty())
            {
                script.Start(this, entity);
            }

            for (const auto& it : script.m_ScriptEntitys)
                it->OnUpdate(dt);
        });

        auto transformView = m_Registry.view<TransformComponent>();
        for (auto entity : transformView)
        {
            transformView.get<TransformComponent>(entity).Update(dt);
        }

        m_Registry.view<InputComponent>().each([&](InputComponent& inputComponent)
        {
            inputComponent.EndFrame();
        });

        for (const auto view = m_Registry.view<CameraComponent>(); const auto entity : view)
        {
            if (const auto& cam = m_Registry.get<CameraComponent>(entity); cam.Primary)
            {
                RenderCommande::UseCamera(cam.camera);
                break;
            }
        }

        if (!Application::IsHeadless())
        {
            auto group = m_Registry.view<TransformComponent, MeshComponent>();
            for (const auto& entity : group)
            {
                const auto& [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
                RenderCommande::Submit(mesh.m_mesh);
            }
        }
    }

    void World::Draw()
    {
        m_Registry.view<NativeScriptComponent>().each([&](const entt::entity entity, NativeScriptComponent& script)
        {
            for (const auto& it : script.m_ScriptEntitys)
                it->OnDraw();
        });
    }

    Entity World::CreateEntity(const std::string &name)
    {
        Entity entity{this, m_Registry.create()};
        entity.AddComponent<TagComponent>(name);
        return entity;
    }

    void World::OnPeerConnected(PeerId peerId)
    {
        if (!NetworkService::Get().IsServer())
            return;

        NetworkPlayerSessionMessage assignMessage;
        assignMessage.MessageType = NetworkPlayerSessionMessage::Type::AssignLocalPeer;
        assignMessage.Peer = peerId;
        NetworkService::Get().Send(peerId, assignMessage);

        NetworkPlayerSessionMessage joinedMessage;
        joinedMessage.MessageType = NetworkPlayerSessionMessage::Type::PlayerJoined;
        joinedMessage.Peer = peerId;
        NetworkService::Get().Broadcast(joinedMessage);
    }

    void World::OnPeerDisconnected(PeerId peerId)
    {
        if (!NetworkService::Get().IsServer())
            return;

        NetworkPlayerSessionMessage leftMessage;
        leftMessage.MessageType = NetworkPlayerSessionMessage::Type::PlayerLeft;
        leftMessage.Peer = peerId;
        NetworkService::Get().Broadcast(leftMessage);
    }

    void World::OnPlayerSessionMessage(const NetworkPlayerSessionMessage& msg)
    {
        if (NetworkService::Get().IsServer())
            return;
    }
} // Sunset
