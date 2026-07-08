//
// Created by sunvy on 07/07/2026.
//

#include "WorldHierarchyPanel.h"

#include <imgui.h>
Pro#include <glm/gtc/type_ptr.inl>

#include "GameFramework/Components/Component.h"
#include "GameFramework/Components/TransformComponent.h"
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

        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            m_SelectedEntity = {};
        ImGui::End();

        ImGui::Begin("Properties");
        if (m_SelectedEntity)
        {
            DrawComponents(m_SelectedEntity);
        }
        ImGui::End();
    }

    void WorldHierarchyPanel::DrawNodeEntity(const Entity& entity)
    {
        if (const auto tag = entity.GetComponent<TagComponent>())
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

    void WorldHierarchyPanel::DrawComponents(const Entity& entity)
    {
        if (auto* tc = entity.GetComponent<TagComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(TagComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Name"))
            {
                auto& tag = tc->Tag;

                char buffer[256] = {};
                strcpy(buffer, tag.c_str());
                if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
                {
                    tag = std::string(buffer);
                }
                ImGui::TreePop();
            }
        }

        if (auto* tc = entity.GetComponent<TransformComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
            {
                auto& position = tc->Position;
                auto& rotation = tc->Rotation;
                auto& scale = tc->Scale;

                ImGui::DragFloat3("Position", glm::value_ptr(position));
                ImGui::DragFloat3("Rotation", glm::value_ptr(rotation));
                ImGui::DragFloat3("Scale", glm::value_ptr(scale));
                ImGui::TreePop();
            }
        }
    }
} // Sunset