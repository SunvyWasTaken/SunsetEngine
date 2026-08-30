//
// Created by sunvy on 06/07/2026.
//

#include "EditorLayer.h"

#include <imgui.h>

#include "GameFramework/World/Entity.h"
#include "GameFramework/World/World.h"
#include "GameFramework/Components/CameraComponent.h"
#include "GameFramework/Components/InputComponent.h"
#include "Panels/WorldHierarchyPanel.h"
#include "Render/Resources/RenderTarget.h"
#include "Render/Core/BuildRenderScene.h"
#include "Render/Core/Renderer.h"
#include "SaveSystem/SaveSystem.h"

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
        , m_Framebuffer(nullptr)
    {
    }

    EditorLayer::~EditorLayer()
    {
    }


    void EditorLayer::Init()
    {
        Layer::Init();
        m_World = std::make_shared<World>();
        m_WorldHierarchy = std::make_unique<WorldHierarchyPanel>(m_World);
        m_Framebuffer = RenderTarget::Create({1280, 720});
    }

    void EditorLayer::OnUpdate(float dt)
    {
        Layer::OnUpdate(dt);

        m_World->Update(dt);
    }

    void EditorLayer::OnDraw(Renderer* renderer)
    {
        Layer::OnDraw(renderer);

        BuildRenderScene scene;

        m_Framebuffer->Bind();
        m_World->Each<CameraComponent>([&](const Entity&, const CameraComponent& camera)
        {
            if (camera.Primary)
            {
                m_RenderScene.BeginScene(camera.camera);
            }
        });
        scene(*(m_World.get()), m_RenderScene);
        renderer->RenderScene(m_RenderScene);
        m_Framebuffer->UnBind();

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

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New Project")) { /* ... */ }
                if (ImGui::MenuItem("Open"))
                {
                    SaveSystem::Load(SAVE_PATH "World.bin", *(m_World.get()));
                }
                if (ImGui::MenuItem("Save"))
                {
                    SaveSystem::Save(SAVE_PATH "World.bin", *(m_World.get()));
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
        ImGui::Image(m_Framebuffer->GetColorAttachmentRenderID(), size);
        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::Begin("Content");
        ImGui::End();

        ImGui::Begin("Panel");
        if (ImGui::Button("Add Entity"))
        {
            m_World->CreateEntity("Entity");
        }
        ImGui::End();

        m_WorldHierarchy->OnImGuiRender();
    }

    bool EditorLayer::OnEvent(const Event::Type &event)
    {
        m_World->Each<InputComponent>([&](const Entity& entity, InputComponent& comp)
        {
            comp.OnEvent(event);
        });
        return Layer::OnEvent(event);
    }
} // Sunset
