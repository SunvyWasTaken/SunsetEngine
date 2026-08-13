//
// Created by sunvy on 11/08/2026.
//

#pragma once

#include "Audio.h"

namespace Sunset
{
    class AudioSource;

    struct AudioSystem
    {
        friend class Application;
    private:
        static void Init();
        static void Shutdown();
        static void Update();
    public:
        static std::shared_ptr<Audio> CreateAudio(const std::filesystem::path& path);
        static AudioSource* Play2DAudio(const std::shared_ptr<Audio>& audio, bool loop);
    };
} // Sunset
