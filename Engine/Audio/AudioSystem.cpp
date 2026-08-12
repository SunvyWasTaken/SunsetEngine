//
// Created by sunvy on 11/08/2026.
//

#include "AudioSystem.h"

#include "AudioBuffer.h"
#include "AudioSource.h"
#include "AL/alc.h"

namespace
{
    constexpr std::uint8_t NbrAudioSource = 32;

    ALCdevice* device = nullptr;
    ALCcontext* context = nullptr;

    std::vector<std::unique_ptr<Sunset::AudioSource>> m_AudioSources;
    std::uint8_t currAudioSource = 0;

    std::uint8_t GetCurrAudio()
    {
        if (++currAudioSource > NbrAudioSource)
            currAudioSource = 0;
        return currAudioSource;
    }
}

namespace Sunset
{
    void AudioSystem::Init()
    {
        LOG("Engine", info, "AudioSystem Init")
        device = alcOpenDevice(nullptr);
        context = alcCreateContext(device, nullptr);
        if (context)
            alcMakeContextCurrent(context);

        m_AudioSources.reserve(NbrAudioSource);

        for (std::uint8_t i = 0; i < NbrAudioSource; ++i)
        {
            m_AudioSources.emplace_back(std::make_unique<AudioSource>());
        }
    }

    void AudioSystem::Shutdown()
    {
        LOG("Engine", info, "AudioSystem Shutdown")
        m_AudioSources.clear();
        alcCloseDevice(device);
    }

    std::shared_ptr<AudioBuffer> AudioSystem::CreateAudioBuffer(const std::filesystem::path &path)
    {
        auto buffer = std::make_shared<AudioBuffer>();
        buffer->LoadFile(path);
        return buffer;
    }

    AudioSource* AudioSystem::Play2DAudio(const std::shared_ptr<AudioBuffer> &audio, bool loop)
    {
        const auto i = GetCurrAudio();
        m_AudioSources[i]->SetAudio(audio);
        m_AudioSources[i]->SetRelativeToListener(true);
        m_AudioSources[i]->Play(loop);
        return m_AudioSources[i].get();
    }
} // Sunset