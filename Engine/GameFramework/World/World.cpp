//
// Created by sunvy on 03/06/2026.
//

#include "World.h"

#include "GameFramework/Controller.h"
#include "Entity.h"
#include "BaseObject/BaseCube.h"
#include "Core/Application.h"
#include "Core/Input.h"
#include "GameFramework/Components/CameraComponent.h"
#include "GameFramework/Components/TransformComponent.h"
#include "Network/NetworkService.h"
#include "Render/Camera.h"
#include "Render/RenderCommande.h"

namespace Sunset
{
    World::World()
        : m_Registry()
        , m_Controllers()
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

        if (!Application::IsHeadless())
            CreatePlayer(m_LocalPeerId);
    }

    World::~World()
    {
        NetworkService::Shutdown();
        LOG("Engine", trace, "World::~World()")
    }

    void World::Update(float deltatime)
    {
        SS_PROFILE_FUNCTION();
        for (auto& controller : m_Controllers)
        {
            controller.Update(deltatime);
        }

        auto transformView = m_Registry.view<TransformComponent>();
        for (auto entity : transformView)
        {
            transformView.get<TransformComponent>(entity).Update(deltatime);
        }
        if (!Application::IsHeadless())
        {
            auto group = m_Registry.view<TransformComponent, MeshComponent>();
            for (const auto& entity : group)
            {
                const auto& [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
                RenderCommande::Submit(mesh.m_mesh);
            }
            for (auto& m : m_Controllers)
            {
                const auto& transform = m_Registry.get<TransformComponent>(m.GetEntity());
                if (auto cam = m_Registry.try_get<CameraComponent>(m.GetEntity()))
                {
                    Camera& camera = cam->camera;
                    camera.SetPosition(transform.GetLocation());
                    camera.SetForward(transform.GetForwardVector());
                    RenderCommande::UseCamera(camera);
                }
                // DrawCube(transform, glm::vec4(1.0), true);
                PRINTSCREEN("Controller location {}", transform.GetLocation());
            }
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
        if (!NetworkService::Get().IsServer())
            return;

        CreatePlayer(peerId, false);

        NetworkPlayerSessionMessage assignMessage;
        assignMessage.MessageType = NetworkPlayerSessionMessage::Type::AssignLocalPeer;
        assignMessage.Peer = peerId;
        NetworkService::Get().Send(peerId, assignMessage);

        for (const auto& controller : m_Controllers)
        {
            NetworkPlayerSessionMessage joinedMessage;
            joinedMessage.MessageType = NetworkPlayerSessionMessage::Type::PlayerJoined;
            joinedMessage.Peer = controller.GetPeerId();
            NetworkService::Get().Send(peerId, joinedMessage);
        }

        NetworkPlayerSessionMessage joinedMessage;
        joinedMessage.MessageType = NetworkPlayerSessionMessage::Type::PlayerJoined;
        joinedMessage.Peer = peerId;
        NetworkService::Get().Broadcast(joinedMessage);
    }

    void World::OnPeerDisconnected(PeerId peerId)
    {
        if (!NetworkService::Get().IsServer())
            return;

        DestroyPlayer(peerId);

        NetworkPlayerSessionMessage leftMessage;
        leftMessage.MessageType = NetworkPlayerSessionMessage::Type::PlayerLeft;
        leftMessage.Peer = peerId;
        NetworkService::Get().Broadcast(leftMessage);
    }

    Controller & World::GetController(size_t index)
    {
        return m_Controllers[index];
    }

    void World::OnPlayerSessionMessage(const NetworkPlayerSessionMessage& msg)
    {
        if (NetworkService::Get().IsServer())
            return;

        switch (msg.MessageType)
        {
            case NetworkPlayerSessionMessage::Type::AssignLocalPeer:
                SetLocalPeerId(msg.Peer);
                break;
            case NetworkPlayerSessionMessage::Type::PlayerJoined:
                if (msg.Peer != m_LocalPeerId)
                    CreatePlayer(msg.Peer, false);
                break;
            case NetworkPlayerSessionMessage::Type::PlayerLeft:
                if (msg.Peer != m_LocalPeerId)
                    DestroyPlayer(msg.Peer);
                break;
            default:
                break;
        }
    }

    void World::SetLocalPeerId(PeerId peerId)
    {
        if (m_LocalPeerId == peerId)
            return;

        const PeerId previousLocalPeerId = m_LocalPeerId;
        m_LocalPeerId = peerId;

        const auto controller = std::ranges::find_if(m_Controllers, [previousLocalPeerId](const Controller& controller)
        {
            return controller.GetPeerId() == previousLocalPeerId;
        });

        if (controller == m_Controllers.end())
            return;

        controller->SetPeerId(peerId);
        if (auto* transform = controller->GetEntity().GetComponent<TransformComponent>())
            transform->OwnerPeerId = peerId;
    }

    void World::DestroyPlayer(PeerId peer)
    {
        const auto controller = std::ranges::find_if(m_Controllers, [peer](const Controller& controller)
        {
            return controller.GetPeerId() == peer;
        });

        if (controller == m_Controllers.end())
            return;

        if (const Entity entity = controller->GetEntity())
            m_Registry.destroy(entity);

        m_Controllers.erase(controller);
    }

    void World::CreatePlayer(PeerId peer, bool local)
    {
        const auto controller = std::ranges::find_if(m_Controllers, [peer](const Controller& controller)
        {
            return controller.GetPeerId() == peer;
        });

        if (controller != m_Controllers.end())
            return;

        std::unique_ptr<IInputSource> inputSource = nullptr;
        if (local)
            inputSource = std::make_unique<LocalInputSource>();
        else
            inputSource = std::make_unique<NetworkInputSource>(peer);

        Controller playerController(peer, inputSource);

        Entity character = CreateEntity("Player");

        playerController.Possess(character);

        const bool receiveNetworkPosition = !Application::IsHeadless() || local;
        const bool broadcastNetworkPosition = Application::IsHeadless() || local;
        character.AddComponent<TransformComponent>(peer, receiveNetworkPosition, broadcastNetworkPosition);
        if (local)
            character.AddComponent<CameraComponent>();

        m_Controllers.emplace_back(std::move(playerController));
    }
} // Sunset
