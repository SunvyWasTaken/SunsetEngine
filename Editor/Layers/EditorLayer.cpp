//
// Created by sunvy on 06/07/2026.
//

#include "EditorLayer.h"

#include "Core/Application.h"
#include "Core/FileDialog.h"
#include "GameFramework/World/Entity.h"
#include "GameFramework/World/ScriptEntity.h"
#include "GameFramework/World/World.h"
#include "GameFramework/Components/InputComponent.h"
#include "GameFramework/Components/NativeScriptComponent.h"
#include "GameFramework/Components/TransformComponent.h"
#include "Panels/WorldHierarchyPanel.h"
#include "SaveSystem/SaveSystem.h"

#include "Core/AssetImporter.h"

namespace
{
    struct ImGuiDockspaceArgs
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
        SetActiveWorld(std::make_shared<World>());
    }

    void EditorLayer::SetActiveWorld(const std::shared_ptr<World>& world)
    {
        m_World = world;
        if (m_WorldHierarchy)
            m_WorldHierarchy->SetContext(m_World);
        else
            m_WorldHierarchy = std::make_unique<WorldHierarchyPanel>(m_World);

        m_WorldHierarchy->SetSelectedEntity({});
        m_ContentBrowserPanel.SetWorld(m_World);
    }

    void EditorLayer::StartPlayMode()
    {
        if (!std::holds_alternative<EditorState::Edit>(m_EditorState))
            return;

        m_EditorWorld = m_World;
        std::shared_ptr<World> runtimeWorld = m_EditorWorld->Clone();
        SetActiveWorld(runtimeWorld);
        m_World->StartUpdate();
        m_EditorState = EditorState::Play{};
    }

    void EditorLayer::StopPlayMode()
    {
        if (std::holds_alternative<EditorState::Edit>(m_EditorState))
            return;

        m_World->Each<NativeScriptComponent>([](const Entity&, NativeScriptComponent& script)
        {
            script.Stop();
        });

        SetActiveWorld(m_EditorWorld ? m_EditorWorld : std::make_shared<World>());
        m_EditorWorld.reset();
        m_EditorState = EditorState::Edit{};
    }

    void EditorLayer::PausePlayMode()
    {
        if (std::holds_alternative<EditorState::Play>(m_EditorState))
            m_EditorState = EditorState::Pause{};
    }

    void EditorLayer::ResumePlayMode()
    {
        if (std::holds_alternative<EditorState::Pause>(m_EditorState))
            m_EditorState = EditorState::Play{};
    }

    void EditorLayer::OnUpdate(float dt)
    {
        Layer::OnUpdate(dt);

        if (std::holds_alternative<EditorState::Play>(m_EditorState))
        {
            m_World->Update(dt);
            m_World->BeginInput();
        }
    }

    void EditorLayer::OnDraw()
    {
        Layer::OnDraw();

        DrawDockspace();

        DrawToolbar();

        DrawViewportPanel();

        DrawPanels();
    }

    bool EditorLayer::OnEvent(const Event::Type &event)
    {
        // m_World->Each<InputComponent>([&](const Entity& entity, InputComponent& comp)
        // {
        //     comp.OnEvent(event);
        // });
        if (std::holds_alternative<EditorState::Play>(m_EditorState))
            m_World->OnEvent(event);


        return Layer::OnEvent(event);
    }

    void EditorLayer::DrawDockspace()
    {
        static ImGuiDockspaceArgs args;

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

        DrawMenuBar();

        ImGuiID docspace_id = ImGui::GetID("MainDockSpace");
        ImGui::DockSpace(docspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

        ImGui::End();
    }

    void EditorLayer::DrawMenuBar()
    {
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New Project")) { /* ... */ }
                const bool canEditWorldFile = std::holds_alternative<EditorState::Edit>(m_EditorState);
                if (ImGui::MenuItem("Open", nullptr, false, canEditWorldFile))
                {
                    const auto worldPath = FileDialog::OpenFile("Open World", CONTENT_PATH, {{"World files", "*.bin"}});
                    if (worldPath && SaveSystem::Load(*worldPath, *(m_World.get())))
                        m_ContentBrowserPanel.SetCurrentWorldPath(*worldPath);
                }
                if (ImGui::MenuItem("Save", nullptr, false, canEditWorldFile))
                {
                    const std::filesystem::path& currentWorldPath = m_ContentBrowserPanel.GetCurrentWorldPath();
                    if (!currentWorldPath.empty())
                    {
                        SaveSystem::Save(currentWorldPath, *(m_World.get()));
                    }
                    else
                    {
                        std::ranges::fill(m_SaveWorldAsBuffer, '\0');
                        std::strncpy(m_SaveWorldAsBuffer.data(), "World.bin", m_SaveWorldAsBuffer.size() - 1);
                        m_SaveWorldAsError.clear();
                        m_ShouldOpenSaveWorldAsPopup = true;
                    }
                }
                if (ImGui::MenuItem("Import", nullptr, false, canEditWorldFile))
                {
                    const auto assetPath = FileDialog::OpenFile("Asset", SUNSET_RESOURCES);
                    Sunset::AssetImporter::Import(assetPath.value());
                }
                ImGui::EndMenu();
            }
            // if (ImGui::BeginMenu("Edit")) { /* ... */ ImGui::EndMenu(); }
            // if (ImGui::BeginMenu("View")) { /* ... */ ImGui::EndMenu(); }
            ImGui::EndMenuBar();
        }

        DrawSaveWorldAsPopup();
    }

    void EditorLayer::DrawToolbar()
    {
        ImGui::Begin("Toolbar", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        if (std::holds_alternative<EditorState::Edit>(m_EditorState))
        {
            if (ImGui::Button("Play"))
                StartPlayMode();
        }
        else
        {
            if (std::holds_alternative<EditorState::Play>(m_EditorState))
            {
                if (ImGui::Button("Pause"))
                    PausePlayMode();
            }
            else
            {
                if (ImGui::Button("Resume"))
                    ResumePlayMode();
            }

            ImGui::SameLine();
            if (ImGui::Button("Stop"))
                StopPlayMode();
        }

        ImGui::End();
    }

    void EditorLayer::DrawSaveWorldAsPopup()
    {
        if (m_ShouldOpenSaveWorldAsPopup)
        {
            ImGui::OpenPopup("Save World As");
            m_ShouldOpenSaveWorldAsPopup = false;
        }

        if (ImGui::BeginPopupModal("Save World As", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::InputText("Name", m_SaveWorldAsBuffer.data(), m_SaveWorldAsBuffer.size());

            if (!m_SaveWorldAsError.empty())
                ImGui::TextColored(ImVec4(0.95f, 0.25f, 0.25f, 1.0f), "%s", m_SaveWorldAsError.c_str());

            if (ImGui::Button("Save"))
            {
                std::filesystem::path filename = m_SaveWorldAsBuffer.data();
                if (filename.empty())
                {
                    m_SaveWorldAsError = "World filename cannot be empty.";
                }
                else if (filename.is_absolute() || filename.has_parent_path())
                {
                    m_SaveWorldAsError = "World file must be created directly in Content.";
                }
                else
                {
                    if (!filename.has_extension())
                        filename += ".bin";

                    const std::filesystem::path worldPath = std::filesystem::path(CONTENT_PATH) / filename;
                    if (std::filesystem::exists(worldPath))
                    {
                        m_SaveWorldAsError = "A file already has this name.";
                    }
                    else if (SaveSystem::Save(worldPath, *(m_World.get())))
                    {
                        m_ContentBrowserPanel.SetCurrentWorldPath(worldPath);
                        m_SaveWorldAsError.clear();
                        ImGui::CloseCurrentPopup();
                    }
                    else
                    {
                        m_SaveWorldAsError = "Could not save world file.";
                    }
                }
            }

            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
            {
                m_SaveWorldAsError.clear();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void EditorLayer::DrawViewportPanel()
    {
        ImGui::Begin("Viewport");
        ImGui::TextUnformatted("Rendu de scène à implémenter.");
        ImGui::End();
    }

    void EditorLayer::DrawPanels()
    {
        ImGui::Begin("Panel");
        if (ImGui::Button("Add Entity"))
        {
            m_World->CreateEntity("Entity");
        }
        ImGui::End();

        m_WorldHierarchy->OnImGuiRender();
        m_ContentBrowserPanel.OnImGuiRender();
    }
} // Sunset
