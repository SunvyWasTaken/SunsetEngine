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

namespace Sunset
{
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

        void OnDraw() override;

        bool OnEvent(const Event::Type &event) override;

    private:
        void DrawDockspace();
        void DrawMenuBar();
        void DrawToolbar();
        void DrawSaveWorldAsPopup();
        void DrawViewportPanel();
        void DrawPanels();
        void SetActiveWorld(const std::shared_ptr<World>& world);
        void StartPlayMode();
        void StopPlayMode();
        void PausePlayMode();
        void ResumePlayMode();
    private:
        std::shared_ptr<World> m_World;
        std::shared_ptr<World> m_EditorWorld;
        std::unique_ptr<WorldHierarchyPanel> m_WorldHierarchy;
        ContentBrowserPanel m_ContentBrowserPanel;

        std::array<char, 256> m_SaveWorldAsBuffer{};
        std::string m_SaveWorldAsError;
        bool m_ShouldOpenSaveWorldAsPopup = false;
        EditorState::Type m_EditorState = EditorState::Edit{};
    };
} // Sunset
