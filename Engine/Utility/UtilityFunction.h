//
// Created by sunvy on 06/02/2026.
//

#ifndef SUNSETCRAFT_UTILITYFUNCTION_H
#define SUNSETCRAFT_UTILITYFUNCTION_H

#include <nlohmann/json.hpp>

namespace SunsetEngine
{
    struct UtilityFunction
    {
        static bool DoesFileExist(const std::string_view &path);
        static bool LoadJson(const std::string_view& path, nlohmann::json& json);
        static bool SaveJson(const std::string_view& path, const nlohmann::json& json);
    };
}

#endif //SUNSETCRAFT_UTILITYFUNCTION_H