//
// Created by sunvy on 11/08/2026.
//

#pragma once

namespace Sunset
{
    class AudioSource;
    class AudioBuffer;

    struct AudioSystem
    {
        friend class Application;
    private:
        static void Init();
        static void Shutdown();
    public:
        static std::shared_ptr<AudioBuffer> CreateAudioBuffer(const std::filesystem::path& path);
        static AudioSource* Play2DAudio(const std::shared_ptr<AudioBuffer>& audio, bool loop);
    };
} // Sunset