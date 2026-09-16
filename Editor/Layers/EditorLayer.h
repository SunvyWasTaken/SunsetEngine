//
// Created by sunvy on 06/07/2026.
//

#pragma once

#include <array>
#include <imgui.h>
#include <string>

#include "Core/Layer.h"
#include "Panels/ContentBrowserPanel.h"
#include "Render/Core/BuildRenderScene.h"
#include "Render/Core/RenderScene.h"

namespace Sunset
{
    class RenderTarget;
    class World;
    class WorldHierarchyPanel;

    class EditorLayer : public Layer
    {
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
        void DrawSaveWorldAsPopup();
        void DrawViewportPanel();
        void DrawGizmo();
        void SelectEntityFromViewport(const ImVec2& mousePosition);
        void DrawPanels();
    private:
        std::shared_ptr<World> m_World;
        std::unique_ptr<WorldHierarchyPanel> m_WorldHierarchy;
        ContentBrowserPanel m_ContentBrowserPanel;
        std::shared_ptr<RenderTarget> m_Framebuffer;
        RenderScene m_RenderScene;
        BuildRenderScene m_BuildRenderScene;
        Camera m_Camera;

        std::array<char, 256> m_SaveWorldAsBuffer{};
        std::string m_SaveWorldAsError;
        bool m_ShouldOpenSaveWorldAsPopup = false;

        ImVec2 m_ViewportMin = ImVec2(0.0f, 0.0f);
        ImVec2 m_ViewportMax = ImVec2(0.0f, 0.0f);
        ImVec2 viewportSize = ImVec2(1280, 720);
    };
} // Sunset
