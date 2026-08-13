//
// Created by sunvy on 13/08/2026.
//

#include "AudioStream.h"

#include "AudioBuffer.h"

#include <AL/al.h>
#include <sndfile.h>

namespace
{
    constexpr std::size_t StreamBufferCount = 4;
    constexpr std::size_t StreamFramesPerBuffer = 16384;
}

namespace Sunset
{
    struct AudioStream::AudioStreamData
    {
        SNDFILE* file = nullptr;
        SF_INFO info = {};
        std::string filePath;
        std::vector<short> samples;
    };

    AudioStream::AudioStream()
        : data(std::make_unique<AudioStreamData>())
    {
    }

    AudioStream::AudioStream(AudioStream&& other) noexcept
        : m_Buffers(std::move(other.m_Buffers))
        , data(std::move(other.data))
    {
        other.data = std::make_unique<AudioStreamData>();
    }

    AudioStream& AudioStream::operator=(AudioStream&& other) noexcept
    {
        if (this == &other)
            return *this;

        if (data && data->file)
            sf_close(data->file);

        m_Buffers = std::move(other.m_Buffers);
        data = std::move(other.data);
        other.data = std::make_unique<AudioStreamData>();
        return *this;
    }

    AudioStream::~AudioStream()
    {
        if (data && data->file)
            sf_close(data->file);
    }

    std::string AudioStream::GetName() const
    {
        return data->filePath;
    }

    void AudioStream::LoadFile(const std::filesystem::path& path)
    {
        if (data->file)
        {
            sf_close(data->file);
            data->file = nullptr;
        }

        data->info = {};
        data->file = sf_open(path.c_str(), SFM_READ, &data->info);
        if (!data->file)
            throw std::runtime_error("Impossible d'ouvrir le fichier audio : " + path.string() + "(" + sf_strerror(nullptr) + ")");

        data->filePath = path.string();
        data->samples.resize(StreamFramesPerBuffer * data->info.channels);

        m_Buffers.clear();
        m_Buffers.reserve(StreamBufferCount);
        for (std::size_t i = 0; i < StreamBufferCount; ++i)
        {
            AudioBuffer buffer;
            buffer.Create();
            if (!FillBuffer(buffer.GetBufferID(), false))
                break;

            m_Buffers.emplace_back(std::move(buffer));
        }
    }

    void AudioStream::Reset()
    {
        if (!data->file)
            return;

        sf_seek(data->file, 0, SEEK_SET);

        for (auto& buffer : m_Buffers)
            FillBuffer(buffer.GetBufferID(), false);
    }

    bool AudioStream::FillBuffer(std::uint32_t buffer, bool loop)
    {
        if (!data->file)
            return false;

        sf_count_t samplesRead = sf_read_short(
            data->file,
            data->samples.data(),
            static_cast<sf_count_t>(data->samples.size())
        );

        if (samplesRead <= 0 && loop)
        {
            sf_seek(data->file, 0, SEEK_SET);
            samplesRead = sf_read_short(
                data->file,
                data->samples.data(),
                static_cast<sf_count_t>(data->samples.size())
            );
        }

        if (samplesRead <= 0)
            return false;

        ALenum format = (data->info.channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16);
        alBufferData(buffer, format, data->samples.data(), static_cast<ALsizei>(samplesRead * sizeof(short)), data->info.samplerate);

        return true;
    }
} // Sunset
