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

        AssetMetaData meta;
        meta.type = static_cast<std::uint32_t>(AssetType::Mesh);
        meta.source = assetPath.c_str();

        if (type == AssetType::Mesh)
        {
            YamlSerialization::Serialize(meta,
                std::filesystem::path(CONTENT_PATH) / (assetPath.stem().string() + ".asset"));
        }
    }

    bool AssetImporter::Load(const std::filesystem::path& assetPath)
    {
        if (!std::filesystem::exists(assetPath))
            return false;

        AssetMetaData meta;
        YamlSerialization::Deserialize(meta, assetPath);
    }
} // Sunset