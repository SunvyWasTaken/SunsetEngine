//
// Created by sunvy on 06/02/2026.
//

#pragma once

#include <nlohmann/json.hpp>

namespace Sunset
{
    struct UtilityFunction
    {
        static bool DoesFileExist(const std::string_view &path);
        static bool LoadJson(const std::string_view& path, nlohmann::json& json);
        static bool SaveJson(const std::string_view& path, const nlohmann::json& json);
    };
}
