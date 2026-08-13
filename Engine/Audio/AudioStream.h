//
// Created by sunvy on 13/08/2026.
//

#pragma once

#include "AudioBuffer.h"

namespace Sunset
{
    class AudioStream final
    {
    public:
        AudioStream();
        AudioStream(const AudioStream&) = delete;
        AudioStream& operator=(const AudioStream&) = delete;
        AudioStream(AudioStream&&) noexcept;
        AudioStream& operator=(AudioStream&&) noexcept;

        ~AudioStream();

        std::string GetName() const;
        void LoadFile(const std::filesystem::path& path);
        void Reset();
        bool FillBuffer(std::uint32_t buffer, bool loop);

        std::vector<AudioBuffer> m_Buffers;

    private:
        struct AudioStreamData;
        std::unique_ptr<AudioStreamData> data;
    };
} // Sunset
