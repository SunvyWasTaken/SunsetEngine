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
        AudioBuffer(const AudioBuffer&) = delete;
        AudioBuffer& operator=(const AudioBuffer&) = delete;
        AudioBuffer(AudioBuffer&&) noexcept;
        AudioBuffer& operator=(AudioBuffer&&) noexcept;

        ~AudioBuffer();

        std::uint32_t GetBufferID() const;

        std::string GetName() const;

        void LoadFile(const std::filesystem::path& path);
        void Create();
        void SetData(std::uint32_t channels, std::uint32_t sampleRate, const short* samples, std::size_t sizeInBytes);

    private:

        struct AudioBufferData;
        std::unique_ptr<AudioBufferData> data;

    };
} // Sunset
