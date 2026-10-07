#pragma once

namespace Sunset
{
    struct RenamedPath
    {
        std::filesystem::path oldPath;
        std::filesystem::path newPath;
    };

    // Own one dialog per panel so each caller can handle its own path references.
    class FilesystemRenameDialog
    {
    public:
        void Open(const std::filesystem::path& path, bool isDirectory);
        std::optional<RenamedPath> Render();

    private:
        std::filesystem::path m_Target;
        std::filesystem::path m_Extension;
        std::array<char, 256> m_NameBuffer{};
        std::string m_Error;
        bool m_ShouldOpen = false;
    };
}
