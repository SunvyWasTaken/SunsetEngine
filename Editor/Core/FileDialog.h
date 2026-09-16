//
// Created by sunvy on 16/09/2026.
//

#pragma once

#include <filesystem>
#include <optional>
#include <string_view>

namespace Sunset
{
    struct FileDialog final
    {
        static std::optional<std::filesystem::path> OpenFile(
            std::string_view title,
            const std::filesystem::path& initialDirectory,
            std::string_view extensionFilter
        );
    };
} // Sunset
