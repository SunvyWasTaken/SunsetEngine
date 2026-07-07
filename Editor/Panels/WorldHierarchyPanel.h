//
// Created by sunvy on 07/07/2026.
//

#pragma once

#include "GameFramework/World/Entity.h"
#include "GameFramework/World/World.h"

namespace Sunset
{
    class WorldHierarchyPanel
    {
    public:
        WorldHierarchyPanel(const std::shared_ptr<World>& context);

        void SetContext(const std::shared_ptr<World>& context);

        void OnImGuiRender();
    private:
        void DrawNodeEntity(const Sunset::Entity& entity);
    private:
        std::shared_ptr<World> m_Context;
        Entity m_SelectedEntity;
    };
} // Sunset