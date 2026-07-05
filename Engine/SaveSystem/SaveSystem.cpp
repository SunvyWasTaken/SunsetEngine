//
// Created by sunvy on 04/07/2026.
//

#include "SaveSystem.h"

namespace Sunset
{
    void SaveSystem::CreateFolder(const std::filesystem::path &path)
    {
        if (exists(path))
            return;

        std::filesystem::create_directories(path);
    }
} // Sunset