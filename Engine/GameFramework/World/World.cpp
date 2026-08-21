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
#include "GameFramework/Components/SpriteRenderComponent.h"
#include "GameFramework/Components/TransformComponent.h"
#include "GameFramework/System/CameraSystem.h"
#include "GameFramework/System/IWorldSystem.h"
#include "GameFramework/System/NativeScriptingSystem.h"
#include "GameFramework/System/RenderMeshSystem.h"
#include "GameFramework/System/SpriteRenderSystem.h"
#include "Network/NetworkService.h"
#include "Render/Core/RenderCommand.h"
#include "SaveSystem/SaveSystem.h"

namespace
{
    constexpr std::uint32_t WorldSaveMagic = 0x44574E53; // SNWD
    constexpr std::uint32_t WorldSaveVersion = 1;

    void RegisterEngineSystems(Sunset::World& world)
    {
        world.AddSystem<Sunset::CameraSystem>();
        world.AddSystem<Sunset::NativeScriptingSystem>();
        world.AddSystem<Sunset::SpriteRenderSystem>();
        world.AddSystem<Sunset::RenderMeshSystem>();
    }

    template <typename ComponentType>
    void SaveComponent(Sunset::BinaryOutputArchive& archive, entt::registry& registry, const entt::entity entity)
    {
        bool hasComponent = registry.all_of<ComponentType>(entity);
        archive(hasComponent);

        if (hasComponent)
            archive(registry.get<ComponentType>(entity));
    }

    template <typename ComponentType>
    void LoadComponent(Sunset::BinaryInputArchive& archive, Sunset::Entity& entity)
    {
        bool hasComponent = false;
        archive(hasComponent);

        if (!hasComponent)
            return;

        ComponentType component{};
        archive(component);
        entity.AddComponent<ComponentType>(std::move(component));
    }
}

namespace Sunset
{
    void Serialize(BinaryInputArchive& archive, Camera& camera)
    {
        archive(camera.m_Position);
        archive(camera.m_Forward);
        archive(camera.m_Up);
        archive(camera.m_Yaw);
        archive(camera.m_Pitch);
        archive(camera.m_Fov);
        archive(camera.m_CameraDistance);
        archive(camera.OrthographicSize);
        archive(camera.NearPlaneDistance);
        archive(camera.FarPlaneDistance);
        archive(camera.m_ProjectionType);
    }

    void Serialize(BinaryOutputArchive& archive, Camera& camera)
    {
        archive(camera.m_Position);
        archive(camera.m_Forward);
        archive(camera.m_Up);
        archive(camera.m_Yaw);
        archive(camera.m_Pitch);
        archive(camera.m_Fov);
        archive(camera.m_CameraDistance);
        archive(camera.OrthographicSize);
        archive(camera.NearPlaneDistance);
        archive(camera.FarPlaneDistance);
        archive(camera.m_ProjectionType);
    }

    void Serialize(BinaryInputArchive& archive, TagComponent& component)
    {
        archive(component.Tag);
    }

    void Serialize(BinaryOutputArchive& archive, TagComponent& component)
    {
        archive(component.Tag);
    }

    void Serialize(BinaryInputArchive& archive, TransformComponent& component)
    {
        archive(component.Position);
        archive(component.Rotation);
        archive(component.Scale);
        archive(component.OwnerPeerId);
        archive(component.bSyncPositionInWorld);
        archive(component.bBroadcastPositionInWorld);
    }

    void Serialize(BinaryOutputArchive& archive, TransformComponent& component)
    {
        archive(component.Position);
        archive(component.Rotation);
        archive(component.Scale);
        archive(component.OwnerPeerId);
        archive(component.bSyncPositionInWorld);
        archive(component.bBroadcastPositionInWorld);
    }

    void Serialize(BinaryInputArchive& archive, CameraComponent& component)
    {
        archive(component.Primary);
        archive(component.camera);
    }

    void Serialize(BinaryOutputArchive& archive, CameraComponent& component)
    {
        archive(component.Primary);
        archive(component.camera);
    }

    void Serialize(BinaryInputArchive& archive, InputBindingInfo& binding)
    {
        archive(binding.Type);
        archive(binding.Action);
        archive(binding.KeyboardKey);
        archive(binding.MouseButton);
        archive(binding.PadButton);
        archive(binding.PadAxis);
        archive(binding.Gamepad);
        archive(binding.Scale);
    }

    void Serialize(BinaryOutputArchive& archive, InputBindingInfo& binding)
    {
        archive(binding.Type);
        archive(binding.Action);
        archive(binding.KeyboardKey);
        archive(binding.MouseButton);
        archive(binding.PadButton);
        archive(binding.PadAxis);
        archive(binding.Gamepad);
        archive(binding.Scale);
    }

    void Serialize(BinaryInputArchive& archive, InputComponent& component)
    {
        archive(component.GetBindings());
        component.RebuildMapping();
    }

    void Serialize(BinaryOutputArchive& archive, InputComponent& component)
    {
        archive(component.GetBindings());
    }

    void Serialize(BinaryInputArchive& archive, SpriteRenderComponent& component)
    {
    }

    void Serialize(BinaryOutputArchive& archive, SpriteRenderComponent& component)
    {
    }

    World::World()
        : m_Registry()
    {
        RegisterEngineSystems(*this);
    }

    World::~World()
    {
        NetworkService::Shutdown();
        LOG("Engine", info, "World::~World()")
    }

    void World::BeginInput()
    {
        m_Registry.view<InputComponent>().each([&](InputComponent& inputComponent)
        {
           inputComponent.BeginFrame();
        });
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

        for (const auto& system : m_Systems)
            system->Update(dt);


        if (!Application::IsHeadless())
        {
            for (const auto view = m_Registry.view<CameraComponent>(); const auto entity : view)
            {
                if (const auto& cam = m_Registry.get<CameraComponent>(entity); cam.Primary)
                {
                    RenderCommand::UseCamera(cam.camera);
                    break;
                }
            }
        }
    }

    Entity World::FindEntityByName(const std::string &name)
    {
        for (const auto& entity : m_Registry.view<TagComponent>())
        {
            auto& tag = m_Registry.get<TagComponent>(entity);
            if (tag.Tag == name)
                return Entity{this, entity};
        }
        return Entity{nullptr};
    }

    Entity World::CreateEntity(const std::string &name)
    {
        Entity entity{this, m_Registry.create()};
        entity.AddComponent<TagComponent>(name);
        return entity;
    }

    void World::DestroyEntity(const Entity& entity)
    {
        if (!entity)
            return;

        const auto id = static_cast<entt::entity>(entity);
        if (m_Registry.valid(id))
            m_Registry.destroy(id);
    }

    void Serialize(BinaryOutputArchive& archive, World& world)
    {
        std::uint32_t magic = WorldSaveMagic;
        std::uint32_t version = WorldSaveVersion;

        archive(magic);
        archive(version);
        archive(world.m_LocalPeerId);

        std::uint64_t entityCount = 0;
        world.ForEach([&entityCount](const Entity&)
        {
            ++entityCount;
        });

        archive(entityCount);

        world.ForEach([&archive, &world](const Entity& entity)
        {
            std::uint32_t entityId = static_cast<std::uint32_t>(entity);
            const auto enttEntity = static_cast<entt::entity>(entity);

            archive(entityId);
            SaveComponent<TagComponent>(archive, world.m_Registry, enttEntity);
            SaveComponent<TransformComponent>(archive, world.m_Registry, enttEntity);
            SaveComponent<CameraComponent>(archive, world.m_Registry, enttEntity);
            SaveComponent<InputComponent>(archive, world.m_Registry, enttEntity);
            SaveComponent<SpriteRenderComponent>(archive, world.m_Registry, enttEntity);
        });
    }

    void Serialize(BinaryInputArchive& archive, World& world)
    {
        std::uint32_t magic = 0;
        std::uint32_t version = 0;

        archive(magic);
        archive(version);

        if (magic != WorldSaveMagic || version != WorldSaveVersion)
        {
            LOG("Engine", error, "Invalid world save file")
            return;
        }

        world.m_Registry.clear();

        archive(world.m_LocalPeerId);

        std::uint64_t entityCount = 0;
        archive(entityCount);

        for (std::uint64_t i = 0; i < entityCount; ++i)
        {
            std::uint32_t entityId = 0;
            archive(entityId);

            Entity entity{&world, world.m_Registry.create(static_cast<entt::entity>(entityId))};
            LoadComponent<TagComponent>(archive, entity);
            LoadComponent<TransformComponent>(archive, entity);
            LoadComponent<CameraComponent>(archive, entity);
            LoadComponent<InputComponent>(archive, entity);
            LoadComponent<SpriteRenderComponent>(archive, entity);
        }
    }
} // Sunset
