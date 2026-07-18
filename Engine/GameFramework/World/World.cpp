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
#include "GameFramework/System/IWorldSystem.h"
#include "GameFramework/System/NativeScriptingSystem.h"
#include "GameFramework/System/RenderMeshSystem.h"
#include "Network/NetworkService.h"
#include "Render/Core/RenderCommand.h"

namespace
{
    void RegisterEngineSystems(Sunset::World& world)
    {
        world.AddSystem<Sunset::NativeScriptingSystem>();
        world.AddSystem<Sunset::RenderMeshSystem>();
    }
}

namespace Sunset
{
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

    Entity World::CreateEntity(const std::string &name)
    {
        Entity entity{this, m_Registry.create()};
        entity.AddComponent<TagComponent>(name);
        return entity;
    }
} // Sunset
