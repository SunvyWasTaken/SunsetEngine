//
// Created by sunvy on 11/08/2026.
//

#include "AudioBuffer.h"

#include "AL/al.h"

#include <sndfile.h>

namespace Sunset
{
    struct AudioBuffer::AudioBufferData
    {
        ALuint buffer = 0;
        std::string filePath;
        std::uint32_t channels = 0, sampleRate = 0, bitsPerSample = 0;
        std::size_t sizeInBytes = 0;
    };

    AudioBuffer::AudioBuffer()
        : data(std::make_unique<AudioBufferData>())
    {
    }

    AudioBuffer::AudioBuffer(AudioBuffer&& other) noexcept
        : data(std::move(other.data))
    {
        other.data = std::make_unique<AudioBufferData>();
    }

    AudioBuffer& AudioBuffer::operator=(AudioBuffer&& other) noexcept
    {
        if (this == &other)
            return *this;

        if (data && data->buffer != 0)
        {
            LOG("Engine", trace, "Audio buffer {} delete", data->buffer)
            alDeleteBuffers(1, &data->buffer);
        }

        data = std::move(other.data);
        other.data = std::make_unique<AudioBufferData>();
        return *this;
    }

    AudioBuffer::~AudioBuffer()
    {
        if (data && data->buffer != 0)
        {
            LOG("Engine", trace, "Audio buffer {} delete", data->buffer)
            alDeleteBuffers(1, &data->buffer);
        }
    }

    std::uint32_t AudioBuffer::GetBufferID() const
    {
        return data->buffer;
        // return 0;
    }

    std::string AudioBuffer::GetName() const
    {
        return data->filePath;
    }

    void AudioBuffer::LoadFile(const std::filesystem::path &path)
    {
        SF_INFO info;
        SNDFILE* file = sf_open(path.c_str(), SFM_READ, &info);
        if (!file)
            throw std::runtime_error("Impossible d'ouvrir le fichier audio : " + path.string() + "(" + sf_strerror(nullptr) + ")");

        std::vector<short> samples(info.frames * info.channels);
        sf_count_t samplesRead = sf_read_short(file, samples.data(), info.frames * info.channels);
        if (samplesRead <= 0)
        {
            sf_close(file);
            throw std::runtime_error("Echec de la lecture du fichier: " + path.string());
        }

        sf_close(file);

        data->filePath = path.string();
        data->sampleRate = info.samplerate;
        data->channels = info.channels;
        data->bitsPerSample = 16;
        data->sizeInBytes = static_cast<std::size_t>(samplesRead) * sizeof(short);

        Create();
        SetData(data->channels, data->sampleRate, samples.data(), data->sizeInBytes);
    }

    void AudioBuffer::Create()
    {
        if (data->buffer != 0)
            return;

        alGenBuffers(1, &data->buffer);
        LOG("Engine", trace, "Audio buffer {} Create", data->buffer)
    }

    void AudioBuffer::SetData(std::uint32_t channels, std::uint32_t sampleRate, const short* samples, std::size_t sizeInBytes)
    {
        data->channels = channels;
        data->sampleRate = sampleRate;
        data->bitsPerSample = 16;
        data->sizeInBytes = sizeInBytes;

        Create();

        ALenum format = (data->channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16);
        alBufferData(data->buffer, format, samples, static_cast<ALsizei>(data->sizeInBytes), static_cast<ALsizei>(data->sampleRate));
    }
} // Sunset
