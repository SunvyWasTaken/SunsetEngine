//
// Created by sunvy on 16/09/2026.
//

#pragma once

namespace Sunset
{
    struct FileDialog final
    {
        struct Filter
        {
            std::string_view label;
            std::string_view pattern;
        };

        static std::optional<std::filesystem::path> OpenFile(
            std::string_view title,
            const std::filesystem::path& initialDirectory,
            std::initializer_list<Filter> filters = {}
        );
    };
} // Sunset
