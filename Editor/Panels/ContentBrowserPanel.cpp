//
// Created by sunvy on 01/09/2026.
//

#include "ContentBrowserPanel.h"

#include "GameFramework/World/World.h"
#include "Render/Resources/Texture.h"
#include "SaveSystem/SaveSystem.h"

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <imgui.h>
#include <string>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace
{
    const std::filesystem::path CONTENT_BROWSER_PATH = CONTENT_PATH;
    std::filesystem::path currentPath = CONTENT_BROWSER_PATH;

    std::unique_ptr<Sunset::Texture> LoadTexture(const char* filepath)
    {
        int width = 0;
        int height = 0;
        int channels = 0;
        stbi_uc* data = stbi_load(filepath, &width, &height, &channels, STBI_rgb_alpha);

        if (!data)
            return nullptr;

        Sunset::TextureDescription desc;
        desc.width = static_cast<std::uint32_t>(width);
        desc.height = static_cast<std::uint32_t>(height);
        desc.format = Sunset::TextureFormat::RGBA;
        desc.data = data;

        std::unique_ptr<Sunset::Texture> texture = Sunset::Texture::Create(desc);
        stbi_image_free(data);

        return texture;
    }

    struct BrowserItemAction
    {
        bool clicked = false;
        bool doubleClicked = false;
    };

    BrowserItemAction DrawBrowserItem(const std::filesystem::directory_entry& entry,
                                      const Sunset::Texture* icon,
                                      const ImVec2& tileSize,
                                      const ImVec2& iconSize)
    {
        const std::string fileName = entry.path().filename().string();
        const std::string itemID = "##ContentBrowserItem_" + entry.path().string();

        ImGui::PushID(itemID.c_str());
        const ImVec2 start = ImGui::GetCursorScreenPos();
        const bool clicked = ImGui::InvisibleButton("ItemButton", tileSize);
        const bool hovered = ImGui::IsItemHovered();
        const bool doubleClicked = hovered && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);
        ImGui::PopID();

        ImDrawList* drawList = ImGui::GetWindowDrawList();
        const ImGuiStyle& style = ImGui::GetStyle();

        const ImU32 backgroundColor = ImGui::GetColorU32(hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
        const ImU32 borderColor = ImGui::GetColorU32(ImGuiCol_Border);
        drawList->AddRectFilled(start, ImVec2(start.x + tileSize.x, start.y + tileSize.y), backgroundColor, style.FrameRounding);
        drawList->AddRect(start, ImVec2(start.x + tileSize.x, start.y + tileSize.y), borderColor, style.FrameRounding);

        if (icon)
        {
            const ImVec2 iconMin(
                start.x + (tileSize.x - iconSize.x) * 0.5f,
                start.y + 8.0f
            );
            const ImVec2 iconMax(iconMin.x + iconSize.x, iconMin.y + iconSize.y);
            drawList->AddImage(
                ImTextureRef(static_cast<ImTextureID>(icon->GetRenderID())),
                iconMin,
                iconMax
            );
        }

        const ImVec2 textSize = ImGui::CalcTextSize(fileName.c_str());
        const float textX = start.x + std::max((tileSize.x - textSize.x) * 0.5f, 6.0f);
        const float textY = start.y + iconSize.y + 14.0f;
        const ImVec2 clipMin(start.x + 6.0f, textY);
        const ImVec2 clipMax(start.x + tileSize.x - 6.0f, start.y + tileSize.y - 6.0f);
        drawList->PushClipRect(clipMin, clipMax, true);
        drawList->AddText(ImVec2(textX, textY), ImGui::GetColorU32(ImGuiCol_Text), fileName.c_str());
        drawList->PopClipRect();

        if (hovered)
            ImGui::SetTooltip("%s", fileName.c_str());

        return {clicked, doubleClicked};
    }
}

namespace Sunset
{
    ContentBrowserPanel::ContentBrowserPanel() = default;
    ContentBrowserPanel::~ContentBrowserPanel() = default;

    void ContentBrowserPanel::SetWorld(const std::shared_ptr<World>& world)
    {
        m_World = world;
    }

    void ContentBrowserPanel::OnImGuiRender()
    {
        if (!m_FolderIcon)
            m_FolderIcon = LoadTexture(SUNSET_EDITOR_LOCAL_RESOURCES "Folder.png");
        if (!m_FileIcon)
            m_FileIcon = LoadTexture(SUNSET_EDITOR_LOCAL_RESOURCES "File.png");

        ImGui::Begin("Content");

        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::Button("Create Folder"))
            {
                std::filesystem::path filepath(currentPath / "NewFolder");
                std::filesystem::create_directory(filepath);
            }
            ImGui::EndPopup();
        }

        if (currentPath != CONTENT_BROWSER_PATH)
        {
            if (ImGui::Button("<-"))
            {
                currentPath = CONTENT_BROWSER_PATH;
            }
        }

        std::vector<std::filesystem::directory_entry> entries;
        for (const auto& entry : std::filesystem::directory_iterator(currentPath))
            entries.push_back(entry);

        std::ranges::sort(entries, [](const std::filesystem::directory_entry& lhs, const std::filesystem::directory_entry& rhs)
        {
            if (lhs.is_directory() != rhs.is_directory())
                return lhs.is_directory() > rhs.is_directory();

            return lhs.path().filename().string() < rhs.path().filename().string();
        });

        const float tileWidth = 96.0f;
        const ImVec2 tileSize(tileWidth, 94.0f);
        const ImVec2 iconSize(54.0f, 54.0f);
        const float padding = 12.0f;
        const float availableWidth = ImGui::GetContentRegionAvail().x;
        const int columns = std::max(1, static_cast<int>(availableWidth / (tileWidth + padding)));

        int column = 0;
        for (const auto& file : entries)
        {
            if (column > 0)
                ImGui::SameLine(0.0f, padding);

            if (file.is_directory())
            {
                const BrowserItemAction action = DrawBrowserItem(file, m_FolderIcon.get(), tileSize, iconSize);

                const std::string popupID = "FolderContext_" + file.path().string();
                if (ImGui::BeginPopupContextItem(popupID.c_str()))
                {
                    if (ImGui::MenuItem("Rename"))
                    {
                        m_RenameTarget = file.path();
                        m_RenameError.clear();

                        const std::string fileName = file.path().filename().string();
                        std::ranges::fill(m_RenameBuffer, '\0');
                        std::strncpy(m_RenameBuffer.data(), fileName.c_str(), m_RenameBuffer.size() - 1);

                        m_ShouldOpenRenamePopup = true;
                    }
                    ImGui::EndPopup();
                }

                if (action.clicked)
                {
                    currentPath = file.path();
                }
            }
            else
            {
                const BrowserItemAction action = DrawBrowserItem(file, m_FileIcon.get(), tileSize, iconSize);
                if (action.doubleClicked && m_World && file.path().extension() == ".bin")
                {
                    SaveSystem::Load(file.path(), *m_World);
                }
            }

            column = (column + 1) % columns;
        }

        if (m_ShouldOpenRenamePopup)
        {
            ImGui::OpenPopup("Rename Folder");
            m_ShouldOpenRenamePopup = false;
        }

        if (ImGui::BeginPopupModal("Rename Folder", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::InputText("Name", m_RenameBuffer.data(), m_RenameBuffer.size());

            if (!m_RenameError.empty())
                ImGui::TextColored(ImVec4(0.95f, 0.25f, 0.25f, 1.0f), "%s", m_RenameError.c_str());

            if (ImGui::Button("Rename"))
            {
                const std::string newName = m_RenameBuffer.data();
                if (newName.empty())
                {
                    m_RenameError = "Folder name cannot be empty.";
                }
                else
                {
                    const std::filesystem::path newPath = m_RenameTarget.parent_path() / newName;
                    if (std::filesystem::exists(newPath))
                    {
                        m_RenameError = "A file or folder already has this name.";
                    }
                    else
                    {
                        std::filesystem::rename(m_RenameTarget, newPath);
                        if (currentPath == m_RenameTarget)
                            currentPath = newPath;

                        m_RenameTarget.clear();
                        m_RenameError.clear();
                        ImGui::CloseCurrentPopup();
                    }
                }
            }

            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
            {
                m_RenameTarget.clear();
                m_RenameError.clear();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        ImGui::End();
    }
} // Sunset
