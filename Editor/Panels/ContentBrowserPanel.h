//
// Created by sunvy on 01/09/2026.
//

#pragma once

#include <array>
#include <filesystem>
#include <memory>
#include <string>

namespace Sunset
{
    class Texture;
    class World;

    class ContentBrowserPanel
    {
    public:
        ContentBrowserPanel();
        ~ContentBrowserPanel();

        void SetWorld(const std::shared_ptr<World>& world);
        void OnImGuiRender();
    private:
        std::shared_ptr<World> m_World;
        std::unique_ptr<Texture> m_FolderIcon;
        std::unique_ptr<Texture> m_FileIcon;
        std::filesystem::path m_RenameTarget;
        std::array<char, 256> m_RenameBuffer{};
        std::string m_RenameError;
        bool m_ShouldOpenRenamePopup = false;
    };
} // Sunset
