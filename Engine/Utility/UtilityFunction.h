//
// Created by sunvy on 06/02/2026.
//

#pragma once

// #include <nlohmann/json.hpp>

namespace Sunset
{
    struct UtilityFunction
    {
        static bool DoesFileExist(const std::filesystem::path &path);

        static std::string OpenTextFile(const std::filesystem::path &path);

        template <typename T>
        requires std::is_arithmetic_v<T>
        static T GetRandInRange(T min, T max);
        // static bool LoadJson(const std::string_view& path, nlohmann::json& json);
        // static bool SaveJson(const std::string_view& path, const nlohmann::json& json);
    };

    template<>
    float UtilityFunction::GetRandInRange(float min, float max);

    template<>
    double UtilityFunction::GetRandInRange(double min, double max);

    template<>
    int UtilityFunction::GetRandInRange(int min, int max);

    template<>
    std::int64_t UtilityFunction::GetRandInRange(std::int64_t min, std::int64_t max);
}
