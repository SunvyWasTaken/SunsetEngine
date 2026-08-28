//
// Created by sunvy on 06/07/2026.
//

#pragma once

#include "Core/Layer.h"

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

        void OnDraw() override;

        bool OnEvent(const Event::Type &event) override;

    private:
        std::shared_ptr<World> m_World;
        std::unique_ptr<WorldHierarchyPanel> m_WorldHierarchy;
        std::shared_ptr<RenderTarget> m_Framebuffer;
    };
} // Sunset