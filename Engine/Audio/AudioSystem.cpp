//
// Created by sunvy on 11/08/2026.
//

#include "AudioSystem.h"

#include "AudioBuffer.h"
#include "AudioStream.h"
#include "AudioSource.h"
#include "AL/alc.h"

#include <sndfile.h>

namespace
{
    constexpr std::uint8_t NbrAudioSource = 32;

    constexpr std::uint64_t MaxAudioSizeForBuffer = 64ULL * 1024ULL; // 64Kib

    ALCdevice* device = nullptr;
    ALCcontext* context = nullptr;

    std::vector<std::unique_ptr<Sunset::AudioSource>> m_AudioSources;
    std::uint8_t currAudioSource = 0;

    std::uint8_t GetCurrAudio()
    {
        if (++currAudioSource >= NbrAudioSource)
            currAudioSource = 0;
        return currAudioSource;
    }

    [[nodiscard]]
    std::uint64_t GetDecodeAudioSize(sf_count_t frames, int channels, int bitsPerSample)
    {
        return static_cast<std::uint64_t>(frames) * static_cast<std::uint64_t>(channels) * static_cast<std::uint64_t>(bitsPerSample) / 8ULL;
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
        if (context)
        {
            alcMakeContextCurrent(nullptr);
            alcDestroyContext(context);
            context = nullptr;
        }

        if (device)
        {
            alcCloseDevice(device);
            device = nullptr;
        }
    }

    void AudioSystem::Update()
    {
        for (const auto& audioSource : m_AudioSources)
            audioSource->Update();
    }

    std::shared_ptr<Audio> AudioSystem::CreateAudio(const std::filesystem::path &path)
    {
        SF_INFO info = {};
        SNDFILE* file = sf_open(path.c_str(), SFM_READ, &info);
        if (!file)
            throw std::runtime_error("Impossible d'ouvrir le fichier audio : " + path.string() + "(" + sf_strerror(nullptr) + ")");

        const auto decodeSize = GetDecodeAudioSize(info.frames, info.channels, 16);
        sf_close(file);

        if (decodeSize <= MaxAudioSizeForBuffer)
        {
            auto audio = std::make_shared<Audio>(std::in_place_type<AudioBuffer>);
            std::get<AudioBuffer>(*audio).LoadFile(path);
            return audio;
        }

        auto audio = std::make_shared<Audio>(std::in_place_type<AudioStream>);
        std::get<AudioStream>(*audio).LoadFile(path);
        return audio;
    }

    AudioSource* AudioSystem::Play2DAudio(const std::shared_ptr<Audio> &audio, bool loop)
    {
        const auto i = GetCurrAudio();
        m_AudioSources[i]->SetAudio(audio);
        m_AudioSources[i]->SetRelativeToListener(true);
        m_AudioSources[i]->Play(loop);
        return m_AudioSources[i].get();
    }
} // Sunset
