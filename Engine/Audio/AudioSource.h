//
// Created by sunvy on 12/08/2026.
//

#pragma once
#include <memory>

namespace Sunset
{
    class AudioBuffer;

    class AudioSource final
    {
    public:
        AudioSource();
        ~AudioSource();

        void SetAudio(const std::shared_ptr<AudioBuffer>& buffer);
        void Play(bool loop = false);
        void Stop();
        void Pause();
        bool IsPlaying() const;

        void SetVolume(float volume) const;

        void SetRelativeToListener(bool relative) const;
    private:
        struct AudioSourceData;
        std::unique_ptr<AudioSourceData> m_AudioSourceData;
        std::shared_ptr<AudioBuffer> m_Buffer;
    };
} // Sunset