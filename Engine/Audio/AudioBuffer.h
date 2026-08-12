//
// Created by sunvy on 11/08/2026.
//

#pragma once

namespace Sunset
{
    class AudioBuffer final
    {
    public:
        AudioBuffer();

        ~AudioBuffer();

        std::uint32_t GetBufferID() const;

        std::string GetName() const;

        void LoadFile(const std::filesystem::path& path);

    private:

        struct AudioBufferData;
        std::unique_ptr<AudioBufferData> data;

    };
} // Sunset