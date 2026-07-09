//
// Created by sunvy on 06/07/2026.
//

#include "EditorLayer.h"

#include <iostream>

#include "GameFramework/Components/InputComponent.h"
#include "GameFramework/Components/NativeScriptComponent.h"
#include "GameFramework/Components/TransformComponent.h"
#include "GameFramework/World/ScriptEntity.h"
#include "Panels/WorldHierarchyPanel.h"

namespace
{
    class PlayerScript : public Sunset::ScriptEntity
    {
    public:
        void OnUpdate(float dt) override
        {
            std::cerr << "Update " << dt << std::endl;
        }
    };
}

namespace Sunset
{
    EditorLayer::EditorLayer()
        : m_World(std::make_shared<World>())
        , m_WorldHierarchy(std::make_unique<WorldHierarchyPanel>(m_World))
    {
        // Entity player = m_World->CreateEntity("Player");
        // player.AddComponent<NativeScriptComponent>().Bind<PlayerScript>();
        // player.AddComponent<TransformComponent>();
        // player.AddComponent<InputComponent>();
    }

    EditorLayer::~EditorLayer()
    {
    }

    void EditorLayer::OnDraw()
    {
        Layer::OnDraw();
        m_WorldHierarchy->OnImGuiRender();
    }
} // Sunset