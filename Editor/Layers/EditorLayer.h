//
// Created by sunvy on 06/07/2026.
//

#pragma once

#include <array>
#include <imgui.h>
#include <memory>
#include <string>
#include <variant>

#include "Core/Layer.h"
#include "Panels/ContentBrowserPanel.h"
#include "Render/Core/BuildRenderScene.h"
#include "Render/Core/RenderScene.h"

namespace Sunset
{
    class RenderTarget;
    class Texture;
    class World;
    class WorldHierarchyPanel;

    class EditorLayer : public Layer
    {
        struct EditorState
        {
            struct Edit {};
            struct Play {};
            struct Pause {};

            using Type = std::variant<Edit, Play, Pause>;
        };

    public:
        EditorLayer();
        ~EditorLayer() override;

        void Init() override;

        void OnUpdate(float dt) override;

        void OnDraw(Renderer* renderer) override;

        bool OnEvent(const Event::Type &event) override;

    private:
        void RenderWorldViewport(Renderer* renderer);
        void DrawDockspace();
        void DrawMenuBar();
        void DrawToolbar();
        void DrawSaveWorldAsPopup();
        void DrawViewportPanel();
        void DrawGizmo();
        void SelectEntityFromViewport(const ImVec2& mousePosition);
        void DrawPanels();
        void SetActiveWorld(const std::shared_ptr<World>& world);
        void StartPlayMode();
        void StopPlayMode();
        void PausePlayMode();
        void ResumePlayMode();
        bool DrawToolbarImageButton(const char* id, const Texture* icon, const char* fallbackLabel);
    private:
        std::shared_ptr<World> m_World;
        std::shared_ptr<World> m_EditorWorld;
        std::unique_ptr<WorldHierarchyPanel> m_WorldHierarchy;
        ContentBrowserPanel m_ContentBrowserPanel;
        std::shared_ptr<RenderTarget> m_Framebuffer;
        RenderScene m_RenderScene;
        BuildRenderScene m_BuildRenderScene;
        Camera m_Camera;

        std::array<char, 256> m_SaveWorldAsBuffer{};
        std::string m_SaveWorldAsError;
        bool m_ShouldOpenSaveWorldAsPopup = false;
        EditorState::Type m_EditorState = EditorState::Edit{};
        std::unique_ptr<Texture> m_PlayIcon;
        std::unique_ptr<Texture> m_PauseIcon;

        ImVec2 m_ViewportMin = ImVec2(0.0f, 0.0f);
        ImVec2 m_ViewportMax = ImVec2(0.0f, 0.0f);
        ImVec2 viewportSize = ImVec2(1280, 720);
    };
} // Sunset
