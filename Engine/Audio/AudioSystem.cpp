//
// Created by sunvy on 11/08/2026.
//

#include "AudioSystem.h"

#include "AudioSource.h"
#include "AL/alc.h"

namespace
{
    constexpr std::uint8_t NbrAudioSource = 32;

    ALCdevice* device = nullptr;
    ALCcontext* context = nullptr;

    std::vector<std::unique_ptr<Sunset::AudioSource>> m_AudioSources;
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
} // Sunset