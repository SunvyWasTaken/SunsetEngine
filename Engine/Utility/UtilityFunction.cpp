//
// Created by sunvy on 06/02/2026.
//

#include "UtilityFunction.h"

#include <fstream>

#define CREATE_PATH(p) std::filesystem::path filepath(p); std::filesystem::create_directory(filepath.parent_path());

bool Sunset::UtilityFunction::DoesFileExist(const std::string_view &path)
{
    return std::filesystem::exists(path);
}

bool Sunset::UtilityFunction::LoadJson(const std::string_view& path, nlohmann::json& json)
{
    std::ifstream file;
    file.open(path.data());

    if (!file.is_open())
    {
        LOG("Engine", error, "Json file couldn't be open : {}", path);
        DEBUG_BREAK();
        return false;
    }

    file >> json;
    return true;
}

bool Sunset::UtilityFunction::SaveJson(const std::string_view &path, const nlohmann::json &json = nlohmann::json::object())
{
    CREATE_PATH(path);

    std::ofstream file(path.data());
    if (!file.is_open())
    {
        LOG("Engine", error,
            "Couldn't create json file : {}", path);
        return false;
    }
    file << json.dump();
    file.close();
    return true;
}
