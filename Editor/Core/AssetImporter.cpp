//
// Created by sunvy on 30/09/2026.
//

#include "AssetImporter.h"
#include "Core/AssetMetaData.h"
#include "Serialization/YamlSerialization.h"

namespace
{
    enum class AssetType
    {
        None,
        Mesh
    };

    struct AssetTypeRegistry
    {
        static AssetType GetTypeFromExtension(const std::string& extension)
        {
            if (extension == ".fbx" || extension == ".obj")
                return AssetType::Mesh;
            return AssetType::None;
        }
    };
}

namespace Sunset
{
    void AssetImporter::Import(const std::filesystem::path& assetPath)
    {
        if (!std::filesystem::exists(assetPath))
            return;

        const AssetType type = AssetTypeRegistry::GetTypeFromExtension(assetPath.extension());

        if (type == AssetType::Mesh)
        {
            YamlSerialization::Serialize({.source = assetPath.c_str()}, CONTENT_PATH + assetPath.stem().string() + ".asset");
        }
    }
} // Sunset