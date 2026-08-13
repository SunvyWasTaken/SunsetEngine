//
// Created by sunvy on 12/08/2026.
//

#pragma once

#include "Audio.h"

namespace Sunset
{
    class AudioBuffer;

    class AudioSource final
    {
        friend class AudioSystem;
    public:
        AudioSource();
        ~AudioSource();

        void SetAudio(const std::shared_ptr<Audio>& buffer);
        void Play(bool loop = false);
        void Stop();
        void Pause();
        bool IsPlaying() const;

        void SetVolume(float volume) const;

        void SetRelativeToListener(bool relative) const;

    private:
        void Update();
    private:
        struct AudioSourceData;
        std::unique_ptr<AudioSourceData> m_AudioSourceData;
        std::shared_ptr<Audio> m_Buffer;
    };
} // Sunset