//
// Created by sunvy on 06/02/2026.
//

#ifndef SUNSETCRAFT_UTILITYFUNCTION_H
#define SUNSETCRAFT_UTILITYFUNCTION_H
#include <nlohmann/json_fwd.hpp>

namespace SunsetEngine
{
    struct UtilityFunction
    {
        static nlohmann::json LoadJson(const std::string_view& path);
    };
}

#endif //SUNSETCRAFT_UTILITYFUNCTION_H