//
// Created by sunvy on 30/09/2026.
//

#ifndef SUNSETENGINE_ASSETMETADATA_H
#define SUNSETENGINE_ASSETMETADATA_H

namespace Sunset
{
    struct AssetMetaData
    {
        std::uint32_t version = 0;
        std::uint64_t handle = 0;
        std::uint32_t type = 0;
        std::string source{};
    };
}

#endif //SUNSETENGINE_ASSETMETADATA_H
