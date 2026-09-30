//
// Created by sunvy on 30/09/2026.
//

#ifndef SUNSETENGINE_ASSETIMPORTER_H
#define SUNSETENGINE_ASSETIMPORTER_H

namespace Sunset
{
    struct AssetImporter
    {
        static void Import(const std::filesystem::path& assetPath);
    };
} // Sunset

#endif //SUNSETENGINE_ASSETIMPORTER_H
