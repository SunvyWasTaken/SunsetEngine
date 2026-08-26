//
// Created by sunvy on 06/02/2026.
//

#include "UtilityFunction.h"

#include <fstream>
#include <random>

#define CREATE_PATH(p) std::filesystem::path filepath(p); std::filesystem::create_directory(filepath.parent_path());

namespace
{
    std::ranlux24_base rng{std::random_device{}()};
}

namespace Sunset
{
    bool UtilityFunction::DoesFileExist(const std::filesystem::path &path)
    {
        return std::filesystem::exists(path);
    }

    std::string UtilityFunction::OpenTextFile(const std::filesystem::path &path)
    {
        std::ifstream file(path);
        std::stringstream stream;
        stream << file.rdbuf();
        return stream.str();
    }

    template<>
    float UtilityFunction::GetRandInRange(const float min, const float max)
    {
        return std::uniform_real_distribution(min, max)(rng);
    }

    template<>
    double UtilityFunction::GetRandInRange(const double min, const double max)
    {
        return std::uniform_real_distribution(min, max)(rng);
    }

    template<>
    int UtilityFunction::GetRandInRange(const int min, const int max)
    {
        return std::uniform_int_distribution(min, max)(rng);
    }

    template<>
    std::int64_t UtilityFunction::GetRandInRange(const std::int64_t min, const std::int64_t max)
    {
        return std::uniform_int_distribution(min, max)(rng);
    }
}

// bool Sunset::UtilityFunction::LoadJson(const std::string_view& path, nlohmann::json& json)
// {
//     std::ifstream file;
//     file.open(path.data());
//
//     if (!file.is_open())
//     {
//         LOG("Engine", error, "Json file couldn't be open : {}", path);
//         DEBUG_BREAK();
//         return false;
//     }
//
//     file >> json;
//     return true;
// }
//
// bool Sunset::UtilityFunction::SaveJson(const std::string_view &path, const nlohmann::json &json = nlohmann::json::object())
// {
//     CREATE_PATH(path);
//
//     std::ofstream file(path.data());
//     if (!file.is_open())
//     {
//         LOG("Engine", error,
//             "Couldn't create json file : {}", path);
//         return false;
//     }
//     file << json.dump();
//     file.close();
//     return true;
// }
