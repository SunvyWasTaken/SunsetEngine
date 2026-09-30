//
// Created by sunvy on 30/09/2026.
//

#include "YamlSerialization.h"
#include "Version.h"

#include <yaml-cpp/yaml.h>

#include "Core/AssetMetaData.h"

namespace Sunset
{
    void YamlSerialization::Serialize(const AssetMetaData& metaData, const std::filesystem::path& path)
    {
        YAML::Emitter out;

        out << YAML::BeginMap;

        out << YAML::Key << "Version";
        out << YAML::Value << SUNSET_VERSION;

        out << YAML::Key << "Handle";
        out << YAML::Value << metaData.handle;

        out << YAML::Key << "Type";
        out << YAML::Value << metaData.type;

        out << YAML::Key << "Source";
        out << YAML::Value << metaData.source;

        out << YAML::EndMap;
        std::ofstream file(path);
        file << out.c_str();
    }
} // Sunset