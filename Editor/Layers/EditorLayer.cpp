//
// Created by sunvy on 06/07/2026.
//

#include "EditorLayer.h"

#include <imgui.h>
#include <memory>

#include "Core/GameInstance.h"
#include "Panels/WorldHierarchyPanel.h"

namespace
{
    struct ImGuiDemoDockspaceArgs
    {
        bool                IsFullscreen = true;
        bool                KeepWindowPadding = false; // Keep WindowPadding to help understand that DockSpace() is a widget inside the window.
        ImGuiDockNodeFlags  DockSpaceFlags  = ImGuiDockNodeFlags_None;
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

        static ImGuiDemoDockspaceArgs args;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
        if (args.IsFullscreen)
        {
            // Fullscreen dockspace: practically the same as calling DockSpaceOverViewport();
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            window_flags |= ImGuiWindowFlags_NoBackground;
        }
        else
        {
            // Floating dockspace
            args.DockSpaceFlags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        if (!args.KeepWindowPadding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("SunsetEngine Editor", nullptr, window_flags);

        if (!args.KeepWindowPadding)
            ImGui::PopStyleVar();
        if (args.IsFullscreen)
            ImGui::PopStyleVar(2);

        ImGuiID docspace_id = ImGui::GetID("MainDockSpace");
        ImGui::DockSpace(docspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New Project")) { /* ... */ }
                if (ImGui::MenuItem("Open")) { /* ... */ }
                if (ImGui::MenuItem("Save")) { /* ... */ }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit")) { /* ... */ ImGui::EndMenu(); }
            if (ImGui::BeginMenu("View")) { /* ... */ ImGui::EndMenu(); }
            ImGui::EndMenuBar();
        }

        m_WorldHierarchy->OnImGuiRender();
    }
} // Sunset