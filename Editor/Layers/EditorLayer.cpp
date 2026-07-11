//
// Created by sunvy on 06/07/2026.
//

#include "EditorLayer.h"

#include <iostream>

#include "Core/GameInstance.h"
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
        : m_WorldHierarchy(nullptr)
    {
    }

    EditorLayer::~EditorLayer()
    {
    }

    void EditorLayer::Init()
    {
        Layer::Init();
        m_WorldHierarchy = std::make_unique<WorldHierarchyPanel>(GetGameInstance()->m_ActiveWorld);
    }

    void EditorLayer::OnDraw()
    {
        Layer::OnDraw();
        m_WorldHierarchy->OnImGuiRender();
    }
} // Sunset