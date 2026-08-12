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

    void AudioSource::SetAudio(const std::shared_ptr<AudioBuffer> &buffer)
    {
        m_Buffer = buffer;
        if (m_Buffer)
            alSourcei(m_AudioSourceData->source, AL_BUFFER, static_cast<ALint>(m_Buffer->GetBufferID()));
        else
            alSourcei(m_AudioSourceData->source, AL_BUFFER, 0);
    }

    void AudioSource::Play(bool loop)
    {
        m_AudioSourceData->isLooping = loop;
        alSourcei(m_AudioSourceData->source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);

        alSourcePlay(m_AudioSourceData->source);
        if (alGetError() != AL_NO_ERROR)
        {
            throw std::runtime_error("Echec de la lecture de la source : " + m_Buffer->GetName());
        }
    }

    void AudioSource::Stop()
    {
        alSourceStop(m_AudioSourceData->source);
        if (alGetError() != AL_NO_ERROR)
            throw std::runtime_error("Echec de l'arret " + m_Buffer->GetName());
    }

    void AudioSource::Pause()
    {
        alSourcePause(m_AudioSourceData->source);
        if (alGetError() != AL_NO_ERROR)
            throw std::runtime_error("Echec de la pause " + m_Buffer->GetName());
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
} // Sunset