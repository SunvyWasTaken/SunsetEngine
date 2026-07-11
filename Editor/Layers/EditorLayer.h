//
// Created by sunvy on 06/07/2026.
//

#pragma once

#include "Core/Layer.h"

namespace Sunset
{
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
    };
} // Sunset