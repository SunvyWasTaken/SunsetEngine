//
// Created by sunvy on 30/09/2026.
//

#ifndef SUNSETENGINE_YAMLSERIALIZATION_H
#define SUNSETENGINE_YAMLSERIALIZATION_H

namespace Sunset
{
    struct AssetMetaData;

    struct YamlSerialization
    {
        static void Serialize(const AssetMetaData& metaData, const std::filesystem::path& path);
    };
} // Sunset

#endif //SUNSETENGINE_YAMLSERIALIZATION_H
