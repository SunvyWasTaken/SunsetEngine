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
        void SetCurrentWorldPath(const std::filesystem::path& path);
        const std::filesystem::path& GetCurrentWorldPath() const;

        void OnImGuiRender();
    private:
        void MoveEntryToDirectory(const std::filesystem::path& sourcePath, const std::filesystem::path& destinationDirectory);

        std::shared_ptr<World> m_World;
        std::filesystem::path m_CurrentWorldPath;
        std::unique_ptr<Texture> m_FolderIcon;
        std::unique_ptr<Texture> m_FileIcon;
        std::filesystem::path m_RenameTarget;
        std::array<char, 256> m_RenameBuffer{};
        std::string m_RenameError;
        std::string m_ContentBrowserError;
        bool m_ShouldOpenRenamePopup = false;
    };
} // Sunset
