//
// Created by sunvy on 07/07/2026.
//

#include "WorldHierarchyPanel.h"

#include <imgui.h>

#include "GameFramework/Components/Component.h"
#include "GameFramework/World/Entity.h"

namespace
{

}

namespace Sunset
{
    WorldHierarchyPanel::WorldHierarchyPanel(const std::shared_ptr<World> &context)
    {
        SetContext(context);
    }

    void WorldHierarchyPanel::SetContext(const std::shared_ptr<World> &context)
    {
        m_Context = context;
    }

    void WorldHierarchyPanel::OnImGuiRender()
    {
        ImGui::Begin("Hierarchy");
        m_Context->m_Registry.view<entt::entity>().each([&](const entt::entity entity)
        {
            const Entity entt{m_Context.get(), entity};
            DrawNodeEntity(entt);
        });
        ImGui::End();
    }

    void WorldHierarchyPanel::DrawNodeEntity(const Sunset::Entity& entity)
    {
        if (const auto tag = entity.GetComponent<Sunset::TagComponent>())
        {
            ImGuiTreeNodeFlags flags = (m_SelectedEntity == entity ?  ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
            bool opened = ImGui::TreeNodeEx((void*)(std::uint64_t)(std::uint32_t)entity, flags, tag->Tag.c_str());
            if (ImGui::IsItemClicked())
            {
                m_SelectedEntity = entity;
            }
            if (opened)
                ImGui::TreePop();
        }
    }
} // Sunset