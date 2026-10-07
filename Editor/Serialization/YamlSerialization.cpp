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

    void YamlSerialization::Deserialize(AssetMetaData& metaData, const std::filesystem::path& path)
    {
        auto in = YAML::LoadFile(path.string());

        metaData.version = in["Version"].as<std::string>();
        metaData.handle = in["Handle"].as<uint64_t>();
        metaData.type = in["Type"].as<uint64_t>();
        metaData.source = in["Source"].as<std::string>();
    }
} // Sunset