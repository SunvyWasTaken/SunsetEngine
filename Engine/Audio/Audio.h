//
// Created by sunvy on 11/08/2026.
//

#ifndef SUNSETENGINE_AUDIO_H
#define SUNSETENGINE_AUDIO_H

#include "AudioBuffer.h"

namespace Sunset
{
    struct Audio
    {
        static AudioBuffer Load(const std::string_view& path);
        static void Play2D(const std::shared_ptr<AudioBuffer>& audio);
        static void Play(const AudioBuffer& audioClip);
    };
}

#endif //SUNSETENGINE_AUDIO_H