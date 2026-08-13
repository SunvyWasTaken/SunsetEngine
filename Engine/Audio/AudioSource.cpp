//
// Created by sunvy on 12/08/2026.
//

#include "AudioSource.h"

#include "AudioBuffer.h"

#include <al.h>

namespace Sunset
{
    struct AudioSource::AudioSourceData
    {
        ALuint source = 0;
        bool isLooping = false;
    };

    AudioSource::AudioSource()
        : m_AudioSourceData(std::make_unique<AudioSourceData>())
        , m_Buffer(nullptr)
    {
        alGenSources(1, &m_AudioSourceData->source);
        if (alGetError() != AL_NO_ERROR)
        {
            m_AudioSourceData.reset();
            throw std::runtime_error("Echec de la creation de la source OpenAL");
        }
    }

    AudioSource::~AudioSource()
    {
        if (m_AudioSourceData->source != 0)
            alDeleteSources(1, &m_AudioSourceData->source);
    }

    void AudioSource::SetAudio(const std::shared_ptr<Audio> &buffer)
    {
        alSourceStop(m_AudioSourceData->source);

        ALint queued = 0;
        alGetSourcei(m_AudioSourceData->source, AL_BUFFERS_QUEUED, &queued);
        while (queued-- > 0)
        {
            ALuint queuedBuffer = 0;
            alSourceUnqueueBuffers(m_AudioSourceData->source, 1, &queuedBuffer);
        }

        m_Buffer = buffer;

        if (!m_Buffer)
        {
            alSourcei(m_AudioSourceData->source, AL_BUFFER, 0);
            return;
        }

        if (std::holds_alternative<AudioBuffer>(*buffer.get()))
            alSourcei(m_AudioSourceData->source, AL_BUFFER, static_cast<ALint>(std::get<AudioBuffer>(*m_Buffer.get()).GetBufferID()));
        else
            alSourcei(m_AudioSourceData->source, AL_BUFFER, 0);

    }

    void AudioSource::Play(bool loop)
    {
        m_AudioSourceData->isLooping = loop;

        std::visit(overloads{
            [&](AudioBuffer& buffer)
            {
                alSourcei(m_AudioSourceData->source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
            },
            [&](AudioStream& stream)
            {
                alSourcei(m_AudioSourceData->source, AL_LOOPING, AL_FALSE);
                stream.Reset();

                const int bufferSize = stream.m_Buffers.size();
                std::vector<ALuint> buffers;
                buffers.reserve(bufferSize);

                for (auto& buffer : stream.m_Buffers)
                {
                    buffers.emplace_back(buffer.GetBufferID());
                }

                if (!buffers.empty())
                    alSourceQueueBuffers(m_AudioSourceData->source, static_cast<ALsizei>(bufferSize), buffers.data());
            }
        }, *m_Buffer.get());

        alSourcePlay(m_AudioSourceData->source);
        if (alGetError() != AL_NO_ERROR)
        {
            std::visit(
                [](const auto& buffer)
                {
                    throw std::runtime_error("Echec de la lecture de la source : " + buffer.GetName());
                }
            ,*m_Buffer.get());
        }
    }

    void AudioSource::Stop()
    {
        alSourceStop(m_AudioSourceData->source);
        if (alGetError() != AL_NO_ERROR)
            std::visit(
            [](const auto& buffer)
            {
                throw std::runtime_error("Echec de l'arret " + buffer.GetName());
            }
        ,*m_Buffer.get());
    }

    void AudioSource::Pause()
    {
        alSourcePause(m_AudioSourceData->source);
        if (alGetError() != AL_NO_ERROR)
            std::visit(
[](const auto& buffer)
        {
            throw std::runtime_error("Echec de la pause " + buffer.GetName());
        }
        ,*m_Buffer.get());
    }

    bool AudioSource::IsPlaying() const
    {
        ALint state;
        alGetSourcei(m_AudioSourceData->source, AL_SOURCE_STATE, &state);
        return (state == AL_PLAYING);
    }

    void AudioSource::SetVolume(float volume) const
    {
        if (volume < 0.0f)
            volume = 0.0f;
        else if (volume > 1.0f)
            volume = 1.0f;

        alSourcei(m_AudioSourceData->source, AL_GAIN, volume);
    }

    void AudioSource::SetRelativeToListener(const bool relative) const
    {
        alSourcei(m_AudioSourceData->source, AL_SOURCE_RELATIVE, relative ? AL_TRUE : AL_FALSE);
    }

    void AudioSource::Update()
    {
        if (!m_AudioSourceData->source || !m_Buffer)
            return;

        if (std::holds_alternative<AudioBuffer>(*m_Buffer.get()))
            return;

        auto& stream = std::get<AudioStream>(*m_Buffer.get());

        ALint processed;
        alGetSourcei(m_AudioSourceData->source, AL_BUFFERS_PROCESSED, &processed);

        while (processed--)
        {
            ALuint buffer = 0;

            alSourceUnqueueBuffers(
                m_AudioSourceData->source,
                1,
                &buffer
            );

            if (stream.FillBuffer(buffer, m_AudioSourceData->isLooping))
            {
                alSourceQueueBuffers(
                    m_AudioSourceData->source,
                    1,
                    &buffer
                );
            }
        }

        ALint queued = 0;
        alGetSourcei(m_AudioSourceData->source, AL_BUFFERS_QUEUED, &queued);
        if (queued > 0 && !IsPlaying())
            alSourcePlay(m_AudioSourceData->source);
    }
} // Sunset
