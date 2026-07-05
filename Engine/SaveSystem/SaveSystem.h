//
// Created by sunvy on 04/07/2026.
//

#pragma once

#include "BinaryArchive.h"

namespace Sunset
{
    struct SaveSystem final
    {
        template <typename T>
        static bool Save(const std::filesystem::path& path, T& data)
        {
            std::ofstream file(path, std::ios::binary);

            if (!file.is_open())
                return false;

            BinaryOutputArchive archive(file);
            archive(data);

            return file.good();
        }

        template <typename T>
        static bool Load(const std::filesystem::path& path, T& data)
        {
            std::ifstream file(path, std::ios::binary);

            if (!file.is_open())
                return false;

            BinaryInputArchive archive(file);
            archive(data);

            return file.good();
        }

        static void CreateFolder(const std::filesystem::path& path);
    };
} // Sunset