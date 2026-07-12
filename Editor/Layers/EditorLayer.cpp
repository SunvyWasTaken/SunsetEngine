//
// Created by sunvy on 06/07/2026.
//

#include "EditorLayer.h"

#include <memory>

#include "Core/GameInstance.h"
#include "Panels/WorldHierarchyPanel.h"

namespace
{

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