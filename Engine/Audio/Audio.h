//
// Created by sunvy on 11/08/2026.
//

#ifndef SUNSETENGINE_AUDIO_H
#define SUNSETENGINE_AUDIO_H

#include "AudioClip.h"

namespace Sunset
{
    struct Audio
    {
        static AudioClip Load(const std::string_view& path);
        static void Play(const AudioClip& audioClip);
    };
}

#endif //SUNSETENGINE_AUDIO_H