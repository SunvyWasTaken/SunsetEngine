//
// Created by sunvy on 06/07/2026.
//

#include "EditorLayer.h"

#include <imgui.h>
#include <memory>

#include "Core/GameInstance.h"
#include "Panels/WorldHierarchyPanel.h"
#include "Render/FrameBuffer.h"
#include "Render/FrameBufferSpecification.h"
#include "Render/Core/RenderAPI.h"
#include "Render/Core/RenderCommand.h"
#include "SaveSystem/SaveSystem.h"

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

        window_flags |= ImGuiWindowFlags_MenuBar;

        ImGui::Begin("SunsetEngine Editor", nullptr, window_flags);

        if (!args.KeepWindowPadding)
            ImGui::PopStyleVar();
        if (args.IsFullscreen)
            ImGui::PopStyleVar(2);

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New Project")) { /* ... */ }
                if (ImGui::MenuItem("Open"))
                {
                    SaveSystem::Load(SAVE_PATH "World.bin", *(GetGameInstance()->m_ActiveWorld.get()));
                }
                if (ImGui::MenuItem("Save"))
                {
                    SaveSystem::Save(SAVE_PATH "World.bin", *(GetGameInstance()->m_ActiveWorld.get()));
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit")) { /* ... */ ImGui::EndMenu(); }
            if (ImGui::BeginMenu("View")) { /* ... */ ImGui::EndMenu(); }
            ImGui::EndMenuBar();
        }

        ImGuiID docspace_id = ImGui::GetID("MainDockSpace");
        ImGui::DockSpace(docspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

        ImGui::End();


        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Viewport");
        const auto& size = ImGui::GetContentRegionAvail();
        FrameBuffer frame_buffer({static_cast<int>(size.x), static_cast<int>(size.y)});
        frame_buffer.Bind();
        ImGui::Image(frame_buffer.GetId().id, size);
        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::Begin("Content");
        ImGui::End();

        ImGui::Begin("Panel");
        if (ImGui::Button("Add Entity"))
        {
            GetGameInstance()->m_ActiveWorld->CreateEntity("Entity");
        }
        ImGui::End();

        m_WorldHierarchy->OnImGuiRender();
    }
} // Sunset
