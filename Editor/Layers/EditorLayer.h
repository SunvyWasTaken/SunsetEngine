//
// Created by sunvy on 06/07/2026.
//

#pragma once

#include "Core/Layer.h"

namespace Sunset
{
    class Framebuffer;
    class World;
    class WorldHierarchyPanel;

    class EditorLayer : public Layer
    {
    public:
        EditorLayer();
        ~EditorLayer() override;
        void Init() override;
        void OnDraw() override;
    private:
        std::unique_ptr<WorldHierarchyPanel> m_WorldHierarchy;
        std::shared_ptr<Framebuffer> m_Framebuffer;
    };
} // Sunset