//
// Created by sunvy on 01/09/2026.
//

#pragma once

#include "Widgets/FilesystemRenameDialog.h"

namespace Sunset
{
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
        void UpdatePathsAfterRename(const RenamedPath& renamed);

        std::shared_ptr<World> m_World;
        std::filesystem::path m_CurrentWorldPath;
        FilesystemRenameDialog m_RenameDialog;
        std::string m_ContentBrowserError;
    };
} // Sunset
